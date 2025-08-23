
#ifndef APP_OPIL_H_
#define APP_OPIL_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
#include "stddef.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Updates measurements data.
 *
 * The internal measurements structure is updated with the new data.
 *
 * @param meas Pointer to measurements data.
 * @param size Size of the measurements data, in bytes.
 * @return Always returns 0.
 */
int32_t appOpilUpdateMeasurements(void *meas, int32_t size);
//-----------------------------------------------------------------------------
int32_t appOpilGetMeasurements(void *meas);
//-----------------------------------------------------------------------------
/**
 * @brief Updates simulation data.
 *
 * The internal simulation data structure is updated with the new data.
 *
 * @param simData Pointer to simulation data.
 * @param size Size of the simulation data, in bytes.
 * @return Always returns 0.
 */
int32_t appOpilUpdateSimData(void *simData, int32_t size);
//-----------------------------------------------------------------------------
int32_t appOpilGetSimData(void **simData, int32_t size);
//-----------------------------------------------------------------------------
void appOpilInitializeControl(void);
//-----------------------------------------------------------------------------
/**
 * @brief Runs the control algorithm.
 */
void appOpilRunControl(void);
//-----------------------------------------------------------------------------
int32_t appOpilUpdateControl(void *control, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Gets control data.
 *
 * The location of the structure holding the control data is stored in control.
 *
 * @param control Address of pointer to hold the address of the
 * 			      control data structure.
 * @return The size of the control data structure, in bytes.
 */
int32_t appOpilGetControl(void **control);
//-----------------------------------------------------------------------------
void appOpilUpdateControllerData(void *data, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Gets controller data.
 *
 * The location of the structure holding the controller data is stored in
 * controllerData.
 *
 * @param controllerData Address of pointer to hold the address of the
 * 			             controller data structure.
 * @return The size of the control data structure, in bytes.
 */
int32_t appOpilGetControllerData(void **controllerData);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* APP_OPIL_H_ */