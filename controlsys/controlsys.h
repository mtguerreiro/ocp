/*
 * @file controlsys.h
 *
 * @brief A generic control system written in C.
 *
 * Introduction
 * ============
 *
 * Controlling any system requires the same conceptual steps: take measurements
 * from the system, compute the control actions and apply them.
 *
 * This library provides a simple state machine that executes these steps
 * sequentially. The following steps are executed:
 *
 * 1. Get measurements
 * 2. Execute control algorithm
 * 3. Apply outputs
 *
 * To keep the state machine flexible, the functions executed at each step are
 * actually callbacks supplied by the user. This has the advantage of
 * decoupling these steps: the get measurements callback just worry about
 * getting physical measurements from the system and saving them to a buffer.
 * The control algorithm just reads the measurements from the measurements
 * buffer and write its outputs to an actuator buffer. Finally, the callback
 * applying the outputs to the physical system is just concerned with reading
 * the actuator buffer and using those values to realize the actuation.
 *
 * A more detailed description of each step is given below.
 *
 * 1. Get measurements
 *    In this step, the physical measurements should be taken. This usually
 * corresponds to reading ADCs and converting the samples to their
 * corresponding physical quantities (for instance, voltage, current,
 * temperature, etc).
 *
 * 2. Execute control algorithm
 *    In this step, the control algorithm is executed. Measurements taken in
 * the previous step are passed to the control algorithm, which computes the
 * proper control signals.
 *
 * 3. Apply outputs
 *    In this step, the control signals are actually applied. This usually
 * corresponds to setting PWM signals, relays, etc.
 *
 * Initializing the system
 * =======================
 *
 * Initializing the system requires providing the necessary callbacks and
 * data buffers. The full documentation of each callback can be found in
 * the documentation of the `controlsysConfig_t` data structure.
 *
 * Running the system
 * ==================
 *
 * To run the control system, the `controlsysRun` should be executed. This
 * function would be executed each time there is new data  available, for
 * example, at the end of conversion of the ADCs. The `controlsys` library
 * should not be responsible for triggering new conversions; the underlying
 * hardware should be responsible for this.
 *
 * Typically, the `controlsysRun` should be placed inside an interrupt that is
 * triggered whenever there is new data available.
 *
 * Interfaces
 * ==========
 *
 * It is important to note that the `controlsys` library does not run any
 * control algorithm itself, it merely calls a callback, passing the
 * measurements and expecting that the control outputs will be written in the
 * output buffer. Likewise, the library does not actually performs any
 * measurement or actuation. These are provided by callbacks, which could come
 * from a control library (where the controller is actually run) and a hardware
 * library (where the measurements are actually taken).
 *
 * For this reason, the library provides additional callbacks that can be
 * attached to the the controller and hardware libraries, such that the user
 * can interact with them. The main goal here is to provide all functionalities
 * through a single interface, the `controlsys` library.
 *
 * Status
 * ======
 *
 * Each `controlsys` has a status. This status controls the execution of the
 * state machine of the `controlsysRun` function.
 *
 * If the status is `DISABLED`, the state machine of the `controlsysRun`
 * function will not run. The status is set to disable when a `controlsys`
 * is initialized, and by calling the `controlsysDisable` function.
 *
 * If the status is `ENABLED`, `controlsysRun` executes its state machine,
 * and may change the status to `HARDWARE_ERROR` or `CONTROLLER_ERROR`
 * depending of the execution of the controller and get measurements functions.
 *
 * If the status is `HARDWARE_ERROR` or `CONTROLLER_ERROR` error, the control
 * algorithm is not executed, and the callback to apply the outputs is never
 * called. However, the callback to get measurements is always called. In this
 * way, it is possible to keep tracking the measurements even after an error
 * has occurred.
 *
 * The status of `controlsys` can only be set to `ENABLED` by calling
 * `controlsysEnable`. However, to ensure that it is safe to enable the
 * control system again, `controlsys` is only enabled if the hardware and the
 * controller status are cleared. For this reason, a callback to get the status
 * of the hardware and of the controller must be provided by the user.
 *
 */

#ifndef CONTROLSYS_H_
#define CONTROLSYS_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
/* Status */
typedef enum controlsysStatus_t{
    CONTROLSYS_STATUS_ENABLED,
    CONTROLSYS_STATUS_DISABLED,
	CONTROLSYS_STATUS_HARDWARE_ERROR,
	CONTROLSYS_STATUS_CONTROLLER_ERROR,
}controlsysStatus_t;

/**
 * @brief On entry hook.
 *
 * Called on entry of `controlsysRun`. Can be set to zero if not used.
 */
typedef void(*controlsysOnEntry_t)(void);

/**
 * @brief Get inputs.
 *
 * Called to get the new measurements.
 *
 * @param inputs Buffer to save the new measurements (inputs)
 *
 * @return The number of inputs saved to the inputs buffer. Can be negative to
 *  indicate a problem with data acquisition. In this case, the status of the
 *  `controlsys` is set to `HARDWARE_ERROR`.
 */
typedef int32_t(*controlsysGetInputs_t)(void *inputs);

/**
 * @brief Runs control algorithm (the controller).
 *
 * @param inputs Buffer holding new input values.
 *
 * @param ninputs Number of inputs.
 *
 * @param outputs Buffer to hold new output values.
 *
 * @param nmaxoutputs Size of outputs buffer.
 *
 * @return Hardware status. If different than zero, the status of the
 * `controlsys` is set to `HARDWARE_ERROR`.
 */
typedef int32_t(*controlsysRun_t)(void *inputs, int32_t ninputs, void *outputs, int32_t nmaxoutputs);

/**
 * @brief Apply inputs.
 *
 * Called to apply the control signals.
 *
 * @param outputs Buffer holding the new outputs.
 *
 * @param n Number of outputs.
 *
 * @return Hardware status. If different than zero, the status of the
 * `controlsys` is set to `HARDWARE_ERROR`.
 */
typedef int32_t(*controlsysApplyOutputs_t)(void *outputs, int32_t n);

/**
 * @brief On exit hook.
 *
 * Called before exiting `controlsysRun`. Can be set to zero if not used.
 */
typedef void(*controlsysOnExit_t)(void);

/**
 * @brief Controller interface.
 *
 * A generic way to query the controller inside `controlsys`.
 *
 * @param in Pointer to buffer holding input data.
 *
 * @param insize Size of the input data, in number of bytes.
 *
 * @param out Pointer to pointer holding the output buffer's address. This is
 * implemented as a pointer to pointer for maximum flexibility. The user can
 * either write the command's response to the buffer given initially, or the
 * user can overwrite the pointer to point to another buffer.
 *
 * @param maxoutsize Size of the initial output buffer.
 *
 * @return The size of the query's response (>= 0) or an error (< 0).
 */
typedef int32_t(*controlsysControllerInterface_t)(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

/**
 * @brief Reads the controller's status
 *
 * This is to read the status of the controller inside `controlsys`. It is used
 * internally to enable `controlsys`, only possible if this callback returns 0.
 *
 * This status can be used by the controller to indicate some issue. For
 * example, an invalid control parameter. By setting its status and providing
 * a callback, the control algorithm won't be executed, and the user can have
 * an idea about the problem by checking the status.
 *
 * @return Status of the controller.
 */
typedef int32_t(*controlsysControllerStatus_t)(void);

/**
 * @brief Hardware interface.
 *
 * A generic way to query the hardware inside `controlsys`.
 *
 * @param in Pointer to buffer holding input data.
 *
 * @param insize Size of the input data, in number of bytes.
 *
 * @param out Pointer to pointer holding the output buffer's address. This is
 * implemented as a pointer to pointer for maximum flexibility. The user can
 * either write the command's response to the buffer given initially, or the
 * user can overwrite the pointer to point to another buffer.
 *
 * @param maxoutsize Size of the initial output buffer.
 *
 * @return The size of the query's response (>= 0) or an error (< 0).
 */
typedef int32_t(*controlsysHardwareInterface_t)(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

/**
 * @brief Reads the hardware's status.
 *
 * This is to read the status of the hardware inside `controlsys`. It is used
 * internally to enable `controlsys`, only possible if this callback returns 0.
 *
 * This status can be used by the hardware to indicate some issue. For
 * example, some measurement exceeds its limits. By setting its status and
 * providing a callback, the control algorithm won't be executed, and the user
 * can have  an idea about the problem by checking the status.
 *
 * @return Status of the hardware.
 */
typedef int32_t(*controlsysHardwareStatus_t)(void);

/**
 * @brief Callback to enable sampling.
 *
 * Called when `controlsysEnable` is called. Typically, this would be a
 * hardware function enabling the mechanism that triggers the ADCs.
 */
typedef void(*controlsysEnable_t)(void);

/**
 * @brief Callback to disable sampling.
 *
 * Called when `controlsysDisable` is called. Typically, this would be a
 * hardware function disabling the mechanism that triggers the ADCs.
 */
typedef void(*controlsysDisable_t)(void);

typedef struct {

    void *binputs;
    void *boutputs;

    controlsysOnEntry_t fonEntry;
    controlsysOnEntry_t fonExit;

    controlsysGetInputs_t fgetInputs;
    controlsysApplyOutputs_t fapplyOutputs;

    controlsysRun_t frun;

	controlsysHardwareInterface_t fhwInterface;
	controlsysHardwareStatus_t fhwStatus;

	controlsysControllerInterface_t fcontrollerInterface;
	controlsysControllerStatus_t fcontrollerStatus;

	controlsysEnable_t fenable;
	controlsysDisable_t fdisable;

	controlsysStatus_t status;
}controlsys_t;

typedef controlsys_t controlsysConfig_t;

/* Error codes */
#define CONTROLSYS_ERR_UNDEF    -1
#define CONTROLSYS_ERR_EN       -2
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Initializes a `controlsys`.
 *
 * @param sys Control system.
 *
 * @param config Structure holding the required callbacks.
 */
void controlsysInitialize(controlsys_t *sys, controlsysConfig_t *config);
//-----------------------------------------------------------------------------
/**
 * @brief Queries the controller interface.
 *
 * @param sys Control system.
 *
 * @param in Pointer to buffer holding input data.
 *
 * @param insize Size of the input data, in number of bytes.
 *
 * @param out Pointer to pointer holding the output buffer's address. This is
 * implemented as a pointer to pointer for maximum flexibility. The user can
 * either write the command's response to the buffer given initially, or the
 * user can overwrite the pointer to point to another buffer.
 *
 * @param maxoutsize Size of the initial output buffer.
 *
 * @return The size of the query's response (>= 0) or an error (< 0).
 */
int32_t controlsysControllerInterface(controlsys_t *sys,
		void *in, uint32_t insize,
		void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
/**
 * @brief Queries the hardware interface.
 *
 * @param sys Control system.
 *
 * @param in Pointer to buffer holding input data.
 *
 * @param insize Size of the input data, in number of bytes.
 *
 * @param out Pointer to pointer holding the output buffer's address. This is
 * implemented as a pointer to pointer for maximum flexibility. The user can
 * either write the command's response to the buffer given initially, or the
 * user can overwrite the pointer to point to another buffer.
 *
 * @param maxoutsize Size of the initial output buffer.
 *
 * @return The size of the query's response (>= 0) or an error (< 0).
 */
int32_t controlsysHardwareInterface(controlsys_t *sys,
		void *in, uint32_t insize,
		void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
/**
 * @brief Runs the control system once.
 *
 * This runs the main state machine of the control system: get measurements,
 * run control algorithm and apply control outputs.
 *
 * Execution of these steps depends on the status of each step. Unless `sys` is
 * disabled, the get measurements step is always executed. The other two steps
 * are only executed if `sys` is enabled, which is only possible if there are
 * no errors (from controller and hardware).
 *
 * Typically, this function would be placed at an interrupt generated when
 * new data is available.
 *
 * @param sys Control system.
 *
 * @return Run status.
 */
int32_t controlsysRun(controlsys_t *sys);
//-----------------------------------------------------------------------------
/**
 * @brief Enables the control system.
 *
 * This function sets the status of `sys` to enable. Setting the status to
 * enabled only succeeds if the callbacks to get the controller and hardware
 * status return 0.
 *
 * @param sys Control system.
 *
 * @return Returns 0 if `sys` was enabled successfully.
 */
int32_t controlsysEnable(controlsys_t *sys);
//-----------------------------------------------------------------------------
/**
 * @brief Disables the control system.
 * *
 * @param sys Control system.
 *
 */
void controlsysDisable(controlsys_t *sys);
//-----------------------------------------------------------------------------
/**
 * @brief Reads the status of the control system.
 *
 * @param sys Control system.
 *
 * @return Status.
 */
int32_t controlsysStatus(controlsys_t *sys);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* CONTROLSYS_H_ */
