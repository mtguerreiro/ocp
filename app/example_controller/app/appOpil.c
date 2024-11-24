
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appOpil.h"

//TODO: appConfig?
#include "stypes.h"
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

	uint8_t *src, *dst;

	dst = (uint8_t *)( &xtMeasurements );
	src = (uint8_t *)( meas );

	while(size--) *dst++ = *src++;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetMeasurements(void *meas){

    int32_t size;

    uint8_t *src, *dst;

    dst = (uint8_t *)( meas );
    src = (uint8_t *)( &xtMeasurements );
    size = sizeof(stypesMeasurements_t);

    while(size--) *dst++ = *src++;

    return sizeof(stypesMeasurements_t);
}
//-----------------------------------------------------------------------------
int32_t appOpilUpdateSimData(void *simData, int32_t size){

	uint8_t *src, *dst;

	dst = (uint8_t *)( &xtSimData );
	src = (uint8_t *)( simData );

	while(size--) *dst++ = *src++;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetSimData(void **simData, int32_t size){

    *simData = (void *)( &xtSimData );

    return sizeof(stypesSimData_t);
}
//-----------------------------------------------------------------------------
int32_t appOpilUpdateControl(void *control, int32_t size){

    uint8_t *src, *dst;
    int32_t n;

    dst = (uint8_t *)( &xtControl );
    src = (uint8_t *)( control );

    n = size;
    while(n--) *dst++ = *src++;

    return size;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetControl(void **control){

	*control = (void *)( &xtControl );

	return sizeof(stypesControl_t);
}
//-----------------------------------------------------------------------------
void appOpilUpdateControllerData(void *data, int32_t size){

    uint8_t *src, *dst;

    dst = (uint8_t *)( &xtControllerData );
    src = (uint8_t *)( data );

    while(size--) *dst++ = *src++;
}
//-----------------------------------------------------------------------------
int32_t appOpilGetControllerData(void **controllerData){

	*controllerData = (void *)( &xtControllerData );

	return sizeof(stypesControllerData_t);
}
//-----------------------------------------------------------------------------
//=============================================================================
