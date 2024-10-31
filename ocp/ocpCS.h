/*
 * @file ocpCS.h
 *
 * @brief A single interface for multiple control systems.
 *
 * The controllers are set in the `ocpCSIDs_t` enum type defined in the
 * `ocpConfig.h` file.
 * 
 * This API essentially holds several instances of a `controlsys` (as many as
 * defined in `ocpConfig.h`), and provides access to them via their IDs.
 *
 * For information on how a `controlsys` behaves, please refer to the 
 * of documentation `controlsys.h`.
 */

#ifndef OCP_CS_H_
#define OCP_CS_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"

#include "../controlsys/controlsys.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================


typedef controlsysConfig_t ocpCSConfig_t;

typedef controlsysStatus_t ocpCSStatus_t;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Initializes a control system.
 *
 * @param id ID of the control system.
 *
 * @param config Structure holding the required callbacks.
 * 
 * @param name Name of the controller.
 */
int32_t ocpCSInitialize(uint32_t id, ocpCSConfig_t *config, char *name);
//-----------------------------------------------------------------------------
/**
 * @brief Queries the controller interface of the control system.
 *
 * @param id ID of the control system.
 *
 * @param in Pointer to buffer holding input data.
 *
 * @param insize Size of the input data, in number of bytes.
 *
 * @param out Pointer to pointer holding the output buffer's address. Usually,
 * the data is written to the buffer pointer by this pointer. However, there
 * can be cases where the controller might point to another buffer where the
 * response is stored.
 *
 * @param maxoutsize Maximum size that the initial output buffer is willing to
 * accept.
 *
 * @return The size of the query's response (>= 0) or an error (< 0).
 */
int32_t ocpCSControllerInterface(uint32_t id,
		void *in, uint32_t insize,
		void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
/**
 * @brief Queries the hardware interface of the control system.
 *
 * @param id ID of the control system.
 *
 * @param in Pointer to buffer holding input data.
 *
 * @param insize Size of the input data, in number of bytes.
 *
 * @param out Pointer to pointer holding the output buffer's address. Usually,
 * the data is written to the buffer pointer by this pointer. However, there
 * can be cases where the hardware might point to another buffer where the
 * response is stored.
 *
 * @param maxoutsize Maximum size that the initial output buffer is willing to
 * accept.
 *
 * @return The size of the query's response (>= 0) or an error (< 0).
 */
int32_t ocpCSHardwareInterface(uint32_t id,
		void *in, uint32_t insize,
		void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
/**
 * @brief Runs the control system once.
 *
 * Typically, this function would be placed at an interrupt generated when
 * new data is available.
 *
 * @param id ID of the control system.
 *
 * @return Run status.
 */
int32_t ocpCSRun(uint32_t id);
//-----------------------------------------------------------------------------
/**
 * @brief Enables the control system.
 * 
 * The control system is only enabled if the are no controller and hardware 
 * errors, which are reported by their respective callbacks.
 * 
 * @param id ID of the control system.
 *
 * @return Returns 0 if the control system was enabled successfully.
 */
int32_t ocpCSEnable(uint32_t id);
//-----------------------------------------------------------------------------
/**
 * @brief Disables the control system.
 * 
 * @param id ID of the control system.
 *
 */
int32_t ocpCSDisable(uint32_t id);
//-----------------------------------------------------------------------------
/**
 * @brief Reads the status of the control system.
 *
 * @param id ID of the control system.
 *
 * @return Status (please refer to `ocpCSStatus_t`).
 */
int32_t ocpCSStatus(uint32_t id, int32_t *status);
//-----------------------------------------------------------------------------
/**
 * @brief Reads the number of available control systems.
 *
 * This function just returns the value of `OCP_CS_END`, defined in 
 * `ocpConfig.h`.
 *
 * @return Status.
 */
int32_t ocpCSGetNumberControllers(void);
//-----------------------------------------------------------------------------
/**
 * @brief Reads the names of the initialized control systems.
 *
 * @param buffer Buffer to write the name of the controllers.
 * 
 * @param maxsize Maximum size of bytes that buffer is willing to accept.
 * 
 * @return Total size of names.
 */
int32_t ocpCSGetControllersNames(char *buffer, int32_t maxsize);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* OCP_CS_H_ */
