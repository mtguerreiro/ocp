
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "cukControlRamp.h"
#include "ocpConfig.h"

#include "cukConfig.h"

/* Controllers */
#include "controller/controller.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static float u = 0.0f;
static float u_ref  = 0.5f;
static float u_step = 0.001;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t cukControlRampInitialize(void){

    return 0;
}
//-----------------------------------------------------------------------------
int32_t cukControlRampSetParams(void *params, uint32_t n){

    float *p = (float *)params;

    u_step = *p++;
    u_ref  = *p++;

    if( u_step < 0 ) u_step = 0.0f;

    if( u_ref  > 1.0f ) u_ref  = 1.0f;
    else if( u_ref  < 0.0f) u_ref  = 0.0f;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t cukControlRampGetParams(void *buffer, uint32_t size){

    float *p = (float *)buffer;

    *p++ = u_step;
    *p++ = u_ref ;

    return 8;
}
//-----------------------------------------------------------------------------
int32_t cukControlRampRun(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    cukConfigMeasurements_t *m = (cukConfigMeasurements_t *)meas;
    cukConfigReferences_t *r = (cukConfigReferences_t *)refs;
    cukConfigControl_t *o = (cukConfigControl_t *)outputs;

    if( u < u_ref  ){
        u = u + u_step;
        if(u > u_ref ) u = u_ref ;
    }

    else{
        u = u - u_step;
        if(u < u_ref ) u = u_ref ;
    }

    o->u = u;

    return sizeof(cukConfigControl_t);
}
//-----------------------------------------------------------------------------
void cukControlRampReset(void){

    u = 0.0f;
}
//-----------------------------------------------------------------------------
void cukControlRampGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = cukControlRampInitialize;
    cbs->run = cukControlRampRun;
    cbs->setParams = cukControlRampSetParams;
    cbs->getParams = cukControlRampGetParams;
    cbs->reset = cukControlRampReset;
    cbs->firstEntry = 0;
    cbs->lastExit = 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
