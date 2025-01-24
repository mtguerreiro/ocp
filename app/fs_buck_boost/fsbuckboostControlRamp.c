
#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "fsbuckboostControlRamp.h"

#include "ocpConfig.h"
#include "ocpTrace.h"

#include "fsbuckboostConfig.h"

/* Controllers */
#include "controller.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static float u = 0.0f;
static float u_upper = 0.5f;
static float uinc = 50.0f / 100000.0f;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlRampInitialize(void){

    return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlRampSetParams(void *params, uint32_t n){

    float *p = (float *)params;

    uinc = *p++;
    u_upper = *p++;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlRampGetParams(void *buffer, uint32_t size){

    float *p = (float *)buffer;

    *p++ = uinc;
    *p++ = u_upper;

    return 8;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlRampRun(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostConfigMeasurements_t *m = (fsbuckboostConfigMeasurements_t *)meas;
    fsbuckboostConfigReferences_t *r = (fsbuckboostConfigReferences_t *)refs;
    fsbuckboostConfigControl_t *o = (fsbuckboostConfigControl_t *)outputs;

    u += uinc;

    if( u >= u_upper ) u = u_upper;
    if( u <= 0.0f ) u = 0.0f;

    o->u = u;

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
void fsbuckboostControlRampReset(void){

    u = 0.0f;
}
//-----------------------------------------------------------------------------
void fsbuckboostControlRampGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = fsbuckboostControlRampInitialize;
    cbs->run = fsbuckboostControlRampRun;
    cbs->setParams = fsbuckboostControlRampSetParams;
    cbs->getParams = fsbuckboostControlRampGetParams;
    cbs->reset = fsbuckboostControlRampReset;
    cbs->firstEntry = 0;
    cbs->lastExit = 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
#endif /* SOC_CPU1 */
