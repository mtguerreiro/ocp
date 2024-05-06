/*
 * boostConfig.h
 *
 */

#ifndef BOOST_CONFIG_H_
#define BOOST_CONFIG_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{

    float v_dc_in;
    float v_dc_out;
    float v_out;
    
    float i_l;
    float i_l_avg;
    float i_o;



}boostConfigMeasurements_t;

typedef struct{
    float u;
    float v_o_reference;
    float e;
    float e_reference;
    float i_o_filt;
}boostConfigControl_t;

typedef struct{
    float v_o;
}boostConfigReferences_t;

typedef struct{

    float v_dc_in_gain;
    float v_dc_in_ofs;

    float v_dc_out_gain;
    float v_dc_out_ofs;

    float v_out_gain;
    float v_out_ofs;
    
    float i_l_gain;
    float i_l_ofs;

    float i_l_avg_gain;
    float i_l_avg_ofs;

    float i_o_gain;
    float i_o_ofs;

    float unit;

}boostConfigMeasGains_t;


#define BOOST_CONFIG_V_DC_IN_GAIN      (float)((0.00804f))
#define BOOST_CONFIG_V_DC_IN_OFFS      (float)((0.02395f))


#define BOOST_CONFIG_V_DC_OUT_GAIN      (float)((0.01215f))
#define BOOST_CONFIG_V_DC_OUT_OFFS     (float)((0.03153f))

#define BOOST_CONFIG_V_OUT_GAIN         (float)((0.01207f))
#define BOOST_CONFIG_V_OUT_OFFS        (float)((0.02956f))

#define BOOST_CONFIG_IL_GAIN            (float)((-0.02506f))
#define BOOST_CONFIG_IL_OFFS           (float)((51.52f))

#define BOOST_CONFIG_IL_AVG_GAIN        (float)((-0.02506f))
#define BOOST_CONFIG_IL_AVG_OFFS       (float)((51.51f))

#define BOOST_CONFIG_IO_AVG_GAIN        (float)((0.01164f))
#define BOOST_CONFIG_IO_AVG_OFFS       (float)((-24.2f))



#define BOOST_CONFIG_L                 ((float)(47e-6))
#define BOOST_CONFIG_C                 ((float)(156.6e-6))

#define BOOST_CONFIG_I_LIM             (10.0f) //originally 10
#define BOOST_CONFIG_V_LIM             (30.0f) //originally 30

#define BOOST_CONFIG_SHUTDOWN_U_DEC     0.005



//=============================================================================

#endif /* BOOST_CONFIG_H_ */
