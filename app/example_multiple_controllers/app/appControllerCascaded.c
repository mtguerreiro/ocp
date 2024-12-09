
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appControllerCascaded.h"

#include "appConfig.h"

/* OCP */
#include "ocp/ocpTrace.h"

/* Controllers */
#include "controller/controller.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================


//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static float ki = 0.0530530303030496;
static float k_ei = 0.0530530303030496;

static float kv = -0.04430925432587546;
static float k_ev = -98.62715371027674;

static float ei = 0.0f;
static float ev = 0.0f;
static float ts = 1.0f / 100000.0f;

static float v_ref = 6.0f;
static float dt = 1.0f / 100000.0f;

static float i_max = 10.0f;
static float i_min = -10.0f;

static float i;
static float v;
static float u;
//=============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerCascadedInit(void){


    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedRun(void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    appMeasurements_t *m = (appMeasurements_t *)meas;
    appControl_t *o = (appControl_t *)outputs;
    appReferences_t *r = (appReferences_t *)refs;

    i = m->i;
    v = m->v_out;
    v_ref = r->vref;

    if( u > 1.0f ) u = 1.0f;
    else if( u < 0.0f ) u = 0.0f;

    o->u = u;

    return sizeof(appControl_t);
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedSetParams(void *params, uint32_t size){

    float *p = (float *)params;

    ki = *p++;
    k_ei = *p++;

    kv = *p++;
    k_ev = *p++;

    i_max = *p++;
    i_min = *p++;

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedGetParams(void *buffer, uint32_t size){

    float *p = (float *)buffer;

    *p++ = ki;
    *p++ = k_ei;

    *p++ = kv;
    *p++ = k_ev;

    *p++ = i_max;
    *p++ = i_min;

    return 24;
}
//-----------------------------------------------------------------------------
void appControllerCascadedReset(void){

}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedFirstEntry(void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedLastExit(void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    return 0;
}
//-----------------------------------------------------------------------------
void appControlCascadedGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = appControllerCascadedInit;
    cbs->run = appControllerCascadedRun;
    cbs->setParams = appControllerCascadedSetParams;
    cbs->getParams = appControllerCascadedGetParams;
    cbs->reset = appControllerCascadedReset;
    cbs->firstEntry = appControllerCascadedFirstEntry;
    cbs->lastExit = appControllerCascadedLastExit;
}
//-----------------------------------------------------------------------------
//=============================================================================
