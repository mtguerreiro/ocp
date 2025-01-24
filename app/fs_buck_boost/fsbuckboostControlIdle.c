
#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "fsbuckboostControlIdle.h"
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
#define FS_BUCK_BOOST_CONTROL_IDLE_DECREMENT    0.005
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlIdleInitialize(void){

	//ocpTraceAddSignal(OCP_TRACE_1, (void *)&i_ac, "Grid current");
    return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlIdleRun(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostConfigMeasurements_t *m = (fsbuckboostConfigMeasurements_t *)meas;
    fsbuckboostConfigControl_t *o = (fsbuckboostConfigControl_t *)outputs;

    o->u = 0;

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlIdleSetParams(void *params, uint32_t size){

	return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlIdleGetParams(void *buffer, uint32_t size){

    return 0;
}
//-----------------------------------------------------------------------------
void fsbuckboostControlIdleReset(void){

}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlIdleFirstEntry(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostHwControllerDisable();

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostControlIdleLastExit(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    fsbuckboostHwControllerEnable();

    return sizeof(fsbuckboostConfigControl_t);
}
//-----------------------------------------------------------------------------
void fsbuckboostControlIdleGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = fsbuckboostControlIdleInitialize;
    cbs->run = fsbuckboostControlIdleRun;
    cbs->setParams = fsbuckboostControlIdleSetParams;
    cbs->getParams = fsbuckboostControlIdleGetParams;
    cbs->reset = fsbuckboostControlIdleReset;
    cbs->firstEntry = fsbuckboostControlIdleFirstEntry;
    cbs->lastExit = fsbuckboostControlIdleLastExit;
}
//-----------------------------------------------------------------------------
//=============================================================================
#endif /* SOC_CPU1 */
