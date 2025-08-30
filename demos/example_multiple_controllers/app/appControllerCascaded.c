
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appControllerCascaded.h"

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
    float k_ei;

    float kv;
    float k_ev;

    float i_max;
    float i_min;

    float dt;
}params_t;
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static float ei_int = 0.0f;
static float ei = 0.0f;
static float ei_1 = 0.0f;

static float ev_int = 0.0f;
static float ev = 0.0f;
static float ev_1 = 0.0f;

static float i_ref = 0.0f;

static float i;
static float v;
static float u;

static params_t params = {
  .ki = 0.03133333474397659f, .k_ei = -131.5843048095703f,
  .kv = 0.25200000405311584f, .k_ev = -295.64471435546875f,
  .i_max = 10.0f, .i_min = -10.f, .dt = 1.0f/100e3
};
//=============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerCascadedInit(void){

    ocpTraceAddSignal(APP_CONFIG_OCP_TRACE_ID, (void *)&i_ref, "Current reference");

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedRun(
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

    /* Voltage controller */
    ev = r->vref - v;
    ev_int = ev_int + (params.dt / 2.0f) * (ev + ev_1);
    ev_1 = ev;

    i_ref = -params.k_ev * ev_int - params.kv * v;
    if( i_ref > params.i_max ) i_ref = params.i_max;
    else if( i_ref < params.i_min ) i_ref = params.i_min;

    /* Current controller */
    ei = i_ref - i;
    ei_int = ei_int + (params.dt / 2.0f) * (ei + ei_1);
    ei_1 = ei;

    u = -params.k_ei * ei_int - params.ki * i;

    if( u > 1.0f ) u = 1.0f;
    else if( u < 0.0f ) u = 0.0f;

    o->u = u;

    return sizeof(appControl_t);
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedSetParams(void *buffer, uint32_t size){

    if( size != sizeof(params_t) ) return -1;
    memcpy( (void *)&params, buffer, sizeof(params_t) );

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedGetParams(void *buffer, uint32_t size){

    if( size < sizeof(params_t) ) return -1;

    memcpy( buffer, (void *)&params, sizeof(params_t) );

    return sizeof(params_t);
}
//-----------------------------------------------------------------------------
void appControllerCascadedReset(void){

}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedFirstEntry(
    void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    (void)meas;
    (void)nmeas;
    (void)refs;
    (void)nrefs;
    (void)outputs;
    (void)nmaxoutputs;

    ei_int = 0.0f;
    ei_1 = 0.0f;

    ev_int = 0.0f;
    ev_1 = 0.0f;

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerCascadedLastExit(
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
