
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "app.h"

#include "stdio.h"

/* OCP config */
#include "ocpConfig.h"

/* Open controller project */
#include "ocp/ocpTrace.h"
#include "ocp/ocpCS.h"
#include "ocp/ocpIf.h"

/* App */
#include "appHw.h"
#include "appController.h"
//============================================================================


//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t appOcpTracesInit(void);
//-----------------------------------------------------------------------------
static int32_t appOcpControlSystemInit(void);
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

#define APP_OCP_CS_ID                           OCP_CS_1
#define APP_OCP_TRACE_ID                        OCP_TRACE_1

#define APP_OCP_CONFIG_TRACE_0_SIZE_ELEMENTS    (32 * 1000)

#define APP_OCP_CONFIG_TRACE_0_NAME_LEN         64
#define APP_OCP_CONFIG_TRACE_0_MAX_SIGNALS      20

#define APP_OCP_CONFIG_INPUT_BUF_SIZE           16
#define APP_OCP_CONFIG_OUTPUT_BUF_SIZE          8

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static char trace0Names[APP_OCP_CONFIG_TRACE_0_NAME_LEN];
static size_t trace0Data[APP_OCP_CONFIG_TRACE_0_MAX_SIGNALS];
static float traceBuffer[APP_OCP_CONFIG_TRACE_0_SIZE_ELEMENTS];

static float bInputs[APP_OCP_CONFIG_INPUT_BUF_SIZE];
static float bOutputs[APP_OCP_CONFIG_OUTPUT_BUF_SIZE];

static float texec = 0.0f;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appInit(void){

    appOcpTracesInit();

    appHwInit();
    appControllerInit();

    appOcpControlSystemInit();

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t appOcpTracesInit(void){

    ocpTraceConfig_t config;

    config.mem = (void *)traceBuffer;
    config.size = APP_OCP_CONFIG_TRACE_0_SIZE_ELEMENTS;
    config.data = (void **)trace0Data;
    config.dataSize = APP_OCP_CONFIG_TRACE_0_MAX_SIGNALS;
    config.names = trace0Names;
    config.namesBufferSize = sizeof(trace0Names);

    ocpTraceInitialize(APP_OCP_TRACE_ID, &config, "App trace");

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t appOcpControlSystemInit(void){

    ocpCSConfig_t config;

    config.binputs = (void *)bInputs;
    config.boutputs = (void *)bOutputs;

    config.fhwInterface = appHwIf;
    config.fhwStatus = appHwStatus;

    config.fgetInputs = appHwGetMeasurements;
    config.fapplyOutputs = appHwUpdateControl;

    config.frun = appControllerRun;
    config.fcontrollerInterface = appControllerIf;
    config.fcontrollerStatus = appControllerStatus;

    config.fenable = 0;
    config.fdisable = 0;

    config.fonEntry = 0;
    config.fonExit = 0;

    ocpCSInitialize(APP_OCP_CS_ID, &config, "App controller");

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
