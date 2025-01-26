
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appController.h"

#include "stypes.h"

#include "appConfig.h"

/* OCP */
#include "ocp/ocpTrace.h"
//============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static float ki = 0.0530530303030496;
static float kv = -0.04430925432587546;
static float k_ev = -98.62715371027674;

static float e = 0.0f;

static float v_ref = 6.0f;
static float dt = 1.0f / 100000.0f;

static float i;
static float v;
static float u;
//============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerInit(void){

    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &i ), "Inductor current");
    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &v ), "Output voltage");
    
    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &u ), "Duty-cycle");
    
    ocpTraceAddSignal(OCP_TRACE_1, (void *)( &v_ref ), "Reference");

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerRun(void *inputs, int32_t ninputs, void *outputs, int32_t nmaxoutputs){
    
    stypesMeasurements_t *meas = (stypesMeasurements_t *)inputs;
    stypesControl_t *out = (stypesControl_t *)outputs;

    i = meas->i;
    v = meas->v_out;

    e = e + dt * (v_ref - v);

    u = - ki * i - kv * v - k_ev * e;

    if( u > 1.0f ) u = 1.0f;
    else if( u < 0.0f ) u = 0.0f;

    out->u = u;

    return sizeof(stypesControl_t);
}
//-----------------------------------------------------------------------------
int32_t appControllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    float *p = (float *)in;

    ki = *p++;
    kv = *p++;
    k_ev = *p++;

    v_ref = *p++;

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerStatus(void){

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

