
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
static float ki = 0.03133333474397659;
static float k_ei = -131.5843048095703;

static float kv = 0.25200000405311584;
static float k_ev = -295.64471435546875;

static float i_max = 10.0f;
static float i_min = -10.0f;

static float ei_int = 0.0f;
static float ei = 0.0f;
static float ei_1 = 0.0f;

static float ev_int = 0.0f;
static float ev = 0.0f;
static float ev_1 = 0.0f;

static float v_ref = 6.0f;
static float dt = 1.0f / 100000.0f;

static float i_ref = 0.0f;

static float i;
static float v;
static float u;
//=============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerCascadedInit(void){

    ocpTraceAddSignal(APP_CONFIG_OCP_TRACE_ID, (void *)&i_ref, "Reference current");

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

    /* Voltage controller */
    ev = r->vref - v;
    ev_int = ev_int + (dt / 2.0f) * (ev + ev_1);
    ev_1 = ev;

    i_ref = -k_ev * ev_int - kv * v;
    if( i_ref > i_max ) i_ref = i_max;
    else if( i_ref < i_min ) i_ref = i_min;

    /* Current controller */
    ei = i_ref - i;
    ei_int = ei_int + (dt / 2.0f) * (ei + ei_1);
    ei_1 = ei;

    u = -k_ei * ei_int - ki * i;

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

    ei_int = 0.0f;
    ei_1 = 0.0f;

    ev_int = 0.0f;
    ev_1 = 0.0f;
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
