
#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "fsbuckboostControlStartup.h"

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
static float ufinal = 0.5f;
static float uinc = 50.0f / 100000.0f;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlStartupInitialize(void){

    return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlStartupSetParams(void *params, uint32_t n){

    float *p = (float *)params;

    uinc = *p++;
    ufinal = *p++;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlStartupGetParams(void *buffer, uint32_t size){

    float *p = (float *)buffer;

    *p++ = uinc;
    *p++ = ufinal;

    return 8;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlStartupRun(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostConfigMeasurements_t *m = (fsbuckboostConfigMeasurements_t *)meas;
    fsbuckboostConfigReferences_t *r = (fsbuckboostConfigReferences_t *)refs;
    fsbuckboostConfigControl_t *o = (fsbuckboostConfigControl_t *)outputs;

    u += uinc;

    if( u >= ufinal ) u = ufinal;
    if( u <= 0.0f ) u = 0.0f;

    o->u = u;

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
void fsbuckboostControlStartupReset(void){

    u = 0.0f;
}
//-----------------------------------------------------------------------------
void fsbuckboostControlStartupGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = fsbuckboostControlStartupInitialize;
    cbs->run = fsbuckboostControlStartupRun;
    cbs->setParams = fsbuckboostControlStartupSetParams;
    cbs->getParams = fsbuckboostControlStartupGetParams;
    cbs->reset = fsbuckboostControlStartupReset;
    cbs->firstEntry = 0;
    cbs->lastExit = 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
#endif /* SOC_CPU1 */
