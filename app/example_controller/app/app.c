
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
#include "ocp/ocpOpil.h"

/* App */
#include "appConfig.h"
#include "appHw.h"
#include "appController.h"
#include "appOpil.h"
//============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t appOcpTracesInit(void);
//-----------------------------------------------------------------------------
static int32_t appOcpControlSystemInit(void);
//-----------------------------------------------------------------------------
static int32_t appOpilInit(void);
//-----------------------------------------------------------------------------
void appAdcIrq(void *callbackRef);
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
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
static uint8_t traceBuffer[APP_CONFIG_OCP_TRACE_0_SIZE_BYTES];

static float bInputs[APP_OCP_CONFIG_INPUT_BUF_SIZE];
static float bOutputs[APP_OCP_CONFIG_OUTPUT_BUF_SIZE];
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appInit(void){

    appOcpTracesInit();

    appHwInit();
    appControllerInit();
    appOpilInit();

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
    config.size = APP_CONFIG_OCP_TRACE_0_SIZE_BYTES;
    config.data = (void **)trace0Data;
    config.names = trace0Names;

    ocpTraceInitialize(APP_CONFIG_OCP_TRACE_ID, &config, "App trace");

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t appOcpControlSystemInit(void){

    ocpCSConfig_t config;

    config.binputs = (void *)bInputs;
    config.boutputs = (void *)bOutputs;

    config.fhwInterface = appHwIf;
    config.fhwStatus = appHwStatus;

    config.fgetInputs = appOpilGetMeasurements;
    config.fapplyOutputs = appOpilUpdateControl;

    config.frun = appControllerRun;
    config.fcontrollerInterface = appControllerIf;
    config.fcontrollerStatus = appControllerStatus;

    config.fenable = 0;
    config.fdisable = 0;

    config.fonEntry = 0;
    config.fonExit = 0;

    ocpCSInitialize(APP_CONFIG_OCP_CS_ID, &config, "App controller");

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t appOpilInit(void){

    ocpOpilConfig_t config;

    config.updateMeas = appOpilUpdateMeasurements;
    config.updateSimData = appOpilUpdateSimData;

    config.initControl = 0;
    config.runControl = appAdcIrq;

    config.getControl = appOpilGetControl;
    config.getControllerData = appOpilGetControllerData;

    ocpOpilInitialize(&config);

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*----------------------------------- IRQ -----------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void appAdcIrq(void *callbackRef){
    
    (void)callbackRef;

    ocpCSRun(APP_CONFIG_OCP_CS_ID);
    ocpTraceSave(APP_CONFIG_OCP_TRACE_ID);
}
//-----------------------------------------------------------------------------
//=============================================================================
