
#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"

#include "ocpConfig.h"
#include "stypes.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

/* IDs for the trace and control system */
#define APP_CONFIG_OCP_CS_ID                    OCP_CS_1
#define APP_CONFIG_OCP_TRACE_ID                 OCP_TRACE_1

/* Trace settings */
#define APP_OCP_CONFIG_TRACE_0_SIZE_ELEMENTS    (32 * 1000)
#define APP_OCP_CONFIG_TRACE_0_NAME_LEN         96
#define APP_OCP_CONFIG_TRACE_0_MAX_SIGNALS      20

/* Measurements/actuators buffer sizes */
#define APP_OCP_CONFIG_INPUT_BUF_SIZE           16
#define APP_OCP_CONFIG_OUTPUT_BUF_SIZE          8

/* Measurements, control and reference types */
typedef stypesMeasurements_t appMeasurements_t;

typedef stypesControl_t appControl_t;

typedef struct{
    float vref;
}appReferences_t;

//=============================================================================

#endif /* APP_CONFIG_H_ */
