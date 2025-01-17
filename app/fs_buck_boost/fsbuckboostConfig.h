
#ifndef FS_BUCK_BOOST_CONFIG_H_
#define FS_BUCK_BOOST_CONFIG_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{
    
    float ii;
    float il;
    float io;
    
    float v_in;
    float v_out;
    float v_dc_out;
    
}fsbuckboostConfigMeasurements_t;

typedef struct{
    float u;
}fsbuckboostConfigControl_t;

typedef struct{
    float v_out;
}fsbuckboostConfigReferences_t;

typedef struct{

    float ii_gain;
    float ii_ofs;

    float il_gain;
    float il_ofs;

    float io_gain;
    float io_ofs;

    float v_in_gain;
    float v_in_ofs;
    
    float v_dc_out_gain;
    float v_dc_out_ofs;

    float v_out_gain;
    float v_out_ofs;
}fsbuckboostConfigMeasGains_t;

#define FS_BUCK_BOOST_CFG_II_GAIN       (1.0f)
#define FS_BUCK_BOOST_CFG_II_OFFS       (0.0f)

#define FS_BUCK_BOOST_CFG_IL_GAIN       (1.0f)
#define FS_BUCK_BOOST_CFG_IL_OFFS       (0.0f)

#define FS_BUCK_BOOST_CFG_IO_GAIN       (1.0f)
#define FS_BUCK_BOOST_CFG_IO_OFFS       (0.0f)

#define FS_BUCK_BOOST_CFG_V_IN_GAIN     (1.0f)
#define FS_BUCK_BOOST_CFG_V_IN_OFFS     (0.0f)

#define FS_BUCK_BOOST_CFG_V_OUT_GAIN    (1.0f)
#define FS_BUCK_BOOST_CFG_V_OUT_OFFS    (0.0f)

#define FS_BUCK_BOOST_CFG_V_DC_OUT_GAIN (1.0f)
#define FS_BUCK_BOOST_CFG_V_DC_OUT_OFFS (0.0f)

#define FS_BUCK_BOOST_CONFIG_SHUTDOWN_U_DEC 0.025
//=============================================================================

#endif /* FS_BUCK_BOOST_CONFIG_H_ */
