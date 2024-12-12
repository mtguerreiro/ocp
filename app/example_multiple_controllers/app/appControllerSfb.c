
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appControllerSfb.h"

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
static float kv = -0.04430925432587546;
static float k_ev = -98.62715371027674;

static float e = 0.0f;

static float ev = 0.0f;
static float ev_1 = 0.0f;

static float ts = 1.0f / 100000.0f;

static float v_ref = 6.0f;
static float dt = 1.0f / 100000.0f;

static float i;
static float v;
static float u;
//=============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerSfbInit(void){

    /* Move this elsewhere */
    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &i ), "Inductor current");
    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &v ), "Output voltage");
    
    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &u ), "Duty-cycle");

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbRun(void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    appMeasurements_t *m = (appMeasurements_t *)meas;
    appControl_t *o = (appControl_t *)outputs;
    appReferences_t *r = (appReferences_t *)refs;

    i = m->i;
    v = m->v_out;
    v_ref = r->vref;

    ev = v_ref - v;
    e = e + (dt / 2.0f) * ( ev + ev_1 );
    ev_1 = ev;

    u = - ki * i - kv * v - k_ev * e;

    if( u > 1.0f ) u = 1.0f;
    else if( u < 0.0f ) u = 0.0f;

    o->u = u;

    return sizeof(appControl_t);
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbSetParams(void *params, uint32_t size){

    float *p = (float *)params;

    ki = *p++;
    kv = *p++;
    k_ev = *p++;

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbGetParams(void *buffer, uint32_t size){

    float *p = (float *)buffer;

    *p++ = ki;
    *p++ = kv;
    *p++ = k_ev;

    return 12;
}
//-----------------------------------------------------------------------------
void appControllerSfbReset(void){

    e = 0.0f;

    ev = 0.0f;
    ev_1 = 0.0f;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbFirstEntry(void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerSfbLastExit(void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

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
