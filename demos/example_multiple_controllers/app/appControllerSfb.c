
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appControllerSfb.h"

#include "appConfig.h"

/* OCP */
#include "ocp/ocpTrace.h"

/* Controllers */
#include "controller/controller.h"

#include "string.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{
    float ki;
    float kv;
    float k_ev;
    float dt;
}params_t;
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static float e = 0.0f;

static float ev = 0.0f;
static float ev_1 = 0.0f;

static float i;
static float v;
static float u;

static params_t params = {
    .ki = 0.02015303075313568f, .kv = -0.07867603003978729f, .k_ev = -4.631767272949219f,
    .dt = 1.0f/100e3
};
//=============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerSfbInit(void){

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbRun(
    void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    (void)nmeas;
    (void)nrefs;
    (void)nmaxoutputs;
    appMeasurements_t *m = (appMeasurements_t *)meas;
    appControl_t *o = (appControl_t *)outputs;
    appReferences_t *r = (appReferences_t *)refs;

    i = m->i;
    v = m->v_out;

    ev = r->vref - v;
    e = e + (params.dt / 2.0f) * ( ev + ev_1 );
    ev_1 = ev;

    u = - params.ki * i - params.kv * v - params.k_ev * e;

    if( u > 1.0f ) u = 1.0f;
    else if( u < 0.0f ) u = 0.0f;

    o->u = u;

    return sizeof(appControl_t);
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbSetParams(void *buffer, uint32_t size){

    if( size != sizeof(params_t) ) return -1;
    memcpy( (void *)&params, buffer, sizeof(params_t) );

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbGetParams(void *buffer, uint32_t size){

    if( size < sizeof(params_t) ) return -1;

    memcpy( buffer, (void *)&params, sizeof(params_t) );

    return sizeof(params_t);
}
//-----------------------------------------------------------------------------
void appControllerSfbReset(void){

}
//-----------------------------------------------------------------------------
int32_t appControllerSfbFirstEntry(
    void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    (void)meas;
    (void)nmeas;
    (void)refs;
    (void)nrefs;
    (void)outputs;
    (void)nmaxoutputs;

    e = 0.0f;

    ev = 0.0f;
    ev_1 = 0.0f;

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbLastExit(
    void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    (void)meas;
    (void)nmeas;
    (void)refs;
    (void)nrefs;
    (void)outputs;
    (void)nmaxoutputs;

    return 0;
}
//-----------------------------------------------------------------------------
void appControlSfbGetCallbacks(void *callbacksBuffer){

    controllerCallbacks_t *cbs = (controllerCallbacks_t * )callbacksBuffer;

    cbs->init = appControllerSfbInit;
    cbs->run = appControllerSfbRun;
    cbs->setParams = appControllerSfbSetParams;
    cbs->getParams = appControllerSfbGetParams;
    cbs->reset = appControllerSfbReset;
    cbs->firstEntry = appControllerSfbFirstEntry;
    cbs->lastExit = appControllerSfbLastExit;
}
//-----------------------------------------------------------------------------
//=============================================================================
