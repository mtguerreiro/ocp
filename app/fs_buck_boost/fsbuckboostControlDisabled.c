
#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "fsbuckboostControlDisabled.h"
#include "fsbuckboostConfig.h"

#include "ocpConfig.h"
#include "ocpTrace.h"

/* Controllers */
#include "controller.h"

#include "fsbuckboostHw.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlDisabledInitialize(void){

	//ocpTraceAddSignal(OCP_TRACE_1, (void *)&i_ac, "Grid current");
    return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlDisabledRun(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostConfigMeasurements_t *m = (fsbuckboostConfigMeasurements_t *)meas;
    fsbuckboostConfigControl_t *o = (fsbuckboostConfigControl_t *)outputs;

    o->u = 0.0f;

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlDisabledSetParams(void *params, uint32_t size){

	return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlDisabledGetParams(void *buffer, uint32_t size){

    return 0;
}
//-----------------------------------------------------------------------------
void fsbuckboostControlDisabledReset(void){

}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlDisabledFirstEntry(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostHwControllerDisable();

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlDisabledLastExit(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostHwControllerEnable();

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
void fsbuckboostControlDisabledGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = fsbuckboostControlDisabledInitialize;
    cbs->run = fsbuckboostControlDisabledRun;
    cbs->setParams = fsbuckboostControlDisabledSetParams;
    cbs->getParams = fsbuckboostControlDisabledGetParams;
    cbs->reset = fsbuckboostControlDisabledReset;
    cbs->firstEntry = fsbuckboostControlDisabledFirstEntry;
    cbs->lastExit = fsbuckboostControlDisabledLastExit;
}
//-----------------------------------------------------------------------------
//=============================================================================
#endif /* SOC_CPU1 */
