
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "app.h"

#include "stdio.h"

/* Open controller project */
#include "ocp/ocpConfig.h"
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

#define APP_OCP_CONFIG_TRACE_0_SIZE_BYTES      (100 * 1024 * 1024)

#define APP_OCP_CONFIG_TRACE_0_NAME_LEN         800
#define APP_OCP_CONFIG_TRACE_0_MAX_SIGNALS      20

#define APP_OCP_CONFIG_INPUT_BUF_SIZE           50
#define APP_OCP_CONFIG_OUTPUT_BUF_SIZE          20

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static char trace0Names[APP_OCP_CONFIG_TRACE_0_NAME_LEN];
static size_t trace0Data[APP_OCP_CONFIG_TRACE_0_MAX_SIGNALS];
uint8_t traceBuffer[APP_OCP_CONFIG_TRACE_0_SIZE_BYTES];

static float bInputs[APP_OCP_CONFIG_INPUT_BUF_SIZE];
static float bOutputs[APP_OCP_CONFIG_OUTPUT_BUF_SIZE];

static float texec = 0.0f;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appInit(void){

    appHwInit();
    appControllerInit();

    appOcpTracesInit();
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
    config.size = APP_OCP_CONFIG_TRACE_0_SIZE_BYTES;
    config.data = (void **)trace0Data;
    config.names = trace0Names;

    ocpTraceInitialize(APP_OCP_TRACE_ID, &config, "App Trace");

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

//=============================================================================
/*----------------------------------- IRQ -----------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void appAdcIrq(void *callbackRef){

    ocpCSRun(APP_OCP_CS_ID);
    ocpTraceSave(APP_OCP_TRACE_ID);
}
//-----------------------------------------------------------------------------
//=============================================================================