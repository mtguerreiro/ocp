
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appOpil.h"

#include "stypes.h"

#include "string.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static stypesMeasurements_t xtMeasurements;
static stypesSimData_t xtSimData;
static stypesControl_t xtControl;
static stypesControllerData_t xtControllerData;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appOpilUpdateMeasurements(void *meas, int32_t size){

    memcpy( (void *)( &xtMeasurements ), meas, size );

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetMeasurements(void *meas){

    memcpy( meas, (void *)( &xtMeasurements ), sizeof(xtMeasurements) );

    return sizeof(xtMeasurements);
}
//-----------------------------------------------------------------------------
int32_t appOpilUpdateSimData(void *simData, int32_t size){

    memcpy( (void *)( &xtSimData ), simData, size );

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetSimData(void **simData, int32_t size){

    (void)size;
    *simData = (void *)( &xtSimData );

    return sizeof(stypesSimData_t);
}
//-----------------------------------------------------------------------------
int32_t appOpilUpdateControl(void *control, int32_t size){

    memcpy( (void *)( &xtControl ), control, size );

    return size;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetControl(void **control){

    *control = (void *)( &xtControl );

    return sizeof(stypesControl_t);
}
//-----------------------------------------------------------------------------
void appOpilUpdateControllerData(void *data, int32_t size){

    memcpy( (void *)( &xtControllerData ), data, size );
}
//-----------------------------------------------------------------------------
int32_t appOpilGetControllerData(void **controllerData){

    *controllerData = (void *)( &xtControllerData );

    return sizeof(xtControllerData);
}
//-----------------------------------------------------------------------------
//=============================================================================
