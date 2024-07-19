
#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "boostControlEnergyInt.h"

#include "ocpConfig.h"
#include "ocpTrace.h"

#include "boostConfig.h"

#include "dmpc.h"
#include "dmpc_defs.h"
#include "dmpc_matrices.h"
#include "mvops.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================

#define IL_LIM  150

static float C = 150e-6;
static float L = 47e-6;

static float vo_r = 0.0f;
static float il_r = 0.0f;

static float yr = 0.0f;
static float y = 0.0f, y_dot = 0.0f;

static float v = 0.0f;

static float k1 = 7743839.640425906f, k2 = 5599.999998136721, k3 = -5375358561.238816;

static float dt = 1.0 / 100e3;

static float e = 0.0f, e_dot = 0.0f;

static float xm[DMPC_CONFIG_NXM] = {0};
static float xm_1[DMPC_CONFIG_NXM] = {0};
static float dx[DMPC_CONFIG_NXM] = {0};

static float ref[DMPC_CONFIG_NY];

static float u[DMPC_CONFIG_NU + DMPC_CONFIG_ND] = {0};
static float u_1[DMPC_CONFIG_NU + DMPC_CONFIG_ND] = {0};
static float du[DMPC_CONFIG_NU + DMPC_CONFIG_ND] = {0};

static float aux1[DMPC_CONFIG_NXM], aux2[DMPC_CONFIG_NXM];

static uint32_t n_iters;

static float phi;

static float phi_max, ir_max;

static uint32_t first_enter = 0;

static float duty = 0.0f;

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void boostControlEnergyMpcInitialize(void){

}
//-----------------------------------------------------------------------------
int32_t boostControlEnergyMpcSetParams(void *params, uint32_t n){

    float *p = (float *)params;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t boostControlEnergyMpcGetParams(void *in, uint32_t insize, void *out, uint32_t maxoutsize){

    float *p = (float *)out;

    return 0;
}
//-----------------------------------------------------------------------------
int32_t boostControlEnergyMpcRun(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs){

    boostConfigMeasurements_t *m = (boostConfigMeasurements_t *)meas;
    boostConfigReferences_t *r = (boostConfigReferences_t *)refs;
    boostConfigControl_t *o = (boostConfigControl_t *)outputs;

    uint32_t i;

    /* References */
    il_r = m->v_out * m->i_o / m->v_dc_in;

    vo_r = r->v_o;

    yr = (1. / 2.) * C * vo_r * vo_r + (1. / 2.) * L * il_r * il_r;

    /* States */
    y = (1. / 2.) * C * m->v_dc_out * m->v_dc_out + (1. / 2.) * L * m->i_l * m->i_l;
    y_dot = m->v_dc_in * m->i_l - m->v_dc_out * m->i_o;

    // -----------------------------------
    // --------------- MPC ---------------
    // -----------------------------------
    if( first_enter == 0 ){
        first_enter = 1;
        xm_1[0] = y;
    }

    /* Update bounds on U */
    DMPC_CONFIG_U_MIN[0] = m->v_dc_in / L * (m->v_dc_in - m->v_dc_out) / 1e6;
    DMPC_CONFIG_U_MAX[0] = m->v_dc_in * m->v_dc_in / L / 1e6;

    /* Update bounds on X */
    DMPC_CONFIG_XM_MIN[0] = -IL_LIM * m->v_dc_in - m->v_dc_out * m->i_o;
    DMPC_CONFIG_XM_MAX[0] =  IL_LIM * m->v_dc_in - m->v_dc_out * m->i_o;

    /* Reference */
    ref[0] = yr;

    /* Assembles state vector */
    xm[0] = y;
    xm[1] = y_dot;

    /* Delay compensation */
    dmpcDelayComp(xm, xm, u);

    /* Optimization */
    dmpcOpt(xm, xm_1, ref, u, &n_iters, du);

    /* Computes u = du + u_1 */
    sumv(u, du, DMPC_CONFIG_NU, u);

    /* Saves variables */
    for(i = 0; i < DMPC_CONFIG_NXM; i++){
        xm_1[i] = xm[i];
    }
    // -----------------------------------

    //v = k1 * y_r - k1 * y - k2 * y_dot;

    v = u[0] * 1e6;

    /* Feedback linearization */
    duty = 1 - 1 / m->v_dc_out * (m->v_dc_in - L / m->v_dc_in * v);

    if( duty  > 1.0f ) duty  = 1.0f;
    else if ( duty  < 0.0f ) duty  = 0.0f;

    o->u = duty;

    return sizeof(boostConfigControl_t);
}
//-----------------------------------------------------------------------------
void boostControlEnergyMpcReset(void){

    e = 0.0f;
}
//-----------------------------------------------------------------------------
//=============================================================================
#endif /* SOC_CPU1 */
