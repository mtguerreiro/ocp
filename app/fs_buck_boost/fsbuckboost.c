

#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "fsbuckboost.h"

/* Open controller project */
#include "ocpConfig.h"
#include "ocpTrace.h"
#include "ocpCS.h"
#include "ocpIf.h"
#include "ocpOpil.h"

/* Controller lib */
#include "../controller/controller.h"

/* Application */

//#include "fsBuckBoostOpil.h"
#include "fsbuckboostController.h"

#include "fsbuckboostHwIf.h"
#include "fsbuckboostHw.h"
#include "fsbuckboostConfig.h"

#include "zynqConfig.h"
#include "zynqAxiAdc.h"

//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeHw(void *intcInst);
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeIpc(void *intcInst);
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeTrace(void);
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeTraceSignals(void);
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeControlSystem(void);
//-----------------------------------------------------------------------------
void fsbuckboostAdcIrq(void *callbackRef);
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#define FS_BUCK_BOOST_CONFIG_TRACE_0_ADDR             ZYNQ_CONFIG_MEM_TRACE_ADR
#define FS_BUCK_BOOST_CONFIG_TRACE_0_SIZE             ZYNQ_CONFIG_MEM_TRACE_SIZE_MAX

#define FS_BUCK_BOOST_CONFIG_TRACE_0_NAME_LEN         500
#define FS_BUCK_BOOST_CONFIG_TRACE_0_MAX_SIGNALS      40

#define FS_BUCK_BOOST_CONFIG_INPUT_BUF_SIZE           50
#define FS_BUCK_BOOST_CONFIG_OUTPUT_BUG_SIZE          20
#define FS_BUCK_BOOST_CONFIG_REFERENCE_BUF_SIZE       20

#define FS_BUCK_BOOST_CONFIG_CS_ID                    OCP_CS_1
#define FS_BUCK_BOOST_CONFIG_TRACE_ID                 OCP_TRACE_1
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static char traceNames[FS_BUCK_BOOST_CONFIG_TRACE_0_NAME_LEN];
static size_t traceData[FS_BUCK_BOOST_CONFIG_TRACE_0_MAX_SIGNALS];

static float bInputs[FS_BUCK_BOOST_CONFIG_INPUT_BUF_SIZE];
static float bOutputs[FS_BUCK_BOOST_CONFIG_OUTPUT_BUG_SIZE];

static float texec_boost = 0.0f, texec_buck = 0.0f;
static float texec_buck_boost = 0.0f;

static float taux = 0.0f;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void fsbuckboostInit(void *intcInst){

    fsbuckboostInitializeTrace();

    fsbuckboostInitializeHw(intcInst);
    fsbuckboostInitializeControlSystem();
}
//-----------------------------------------------------------------------------
//=============================================================================


//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeTrace(void){

    ocpTraceConfig_t config;

    config.mem = (void *)FS_BUCK_BOOST_CONFIG_TRACE_0_ADDR;
    config.size = FS_BUCK_BOOST_CONFIG_TRACE_0_SIZE;
    config.data = (void **)traceData;
    config.names = traceNames;

    ocpTraceInitialize(FS_BUCK_BOOST_CONFIG_TRACE_ID, &config, "FS buck boost trace");

    fsbuckboostInitializeTraceSignals();

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeHw(void *intcInst){

    /* Initialize the hardware */
    fsbuckboostHwInitConfig_t hwConfig;

    hwConfig.intc = intcInst;
    hwConfig.irqhandle = fsbuckboostAdcIrq;

    fsbuckboostHwInitialize(&hwConfig);

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeTraceSignals(void){

    fsbuckboostConfigMeasurements_t *meas;
    fsbuckboostConfigControl_t *outputs;

    /* Adds measurements to trace */
    meas = (fsbuckboostConfigMeasurements_t *)bInputs;

    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &meas->ii, "Input current");
    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &meas->il, "Inductor current");
    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &meas->io, "Output current");

    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &meas->v_in, "Input voltage");
    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &meas->v_dc_out, "Output DC link");
    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &meas->v_out, "Output voltage");

    /* Adds control signals to trace */
    outputs = (fsbuckboostConfigControl_t *)bOutputs;
    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &outputs->u, "Duty-cycle");

    /* Other signals to add */
    ocpTraceAddSignal(FS_BUCK_BOOST_CONFIG_TRACE_ID, &texec_buck_boost, "Exec. time");

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostInitializeControlSystem(void){

    ocpCSConfig_t config;

    /* Initializes controller and hardware interface libs */
    fsbuckboostControllerInit();
    fsbuckboostHwIfInitialize();

    /* Initializes control sys lib */
    config.binputs = (void *)bInputs;
    config.boutputs = (void *)bOutputs;

    config.fhwInterface = fsbuckboostHwIf;
    config.fhwStatus = fsbuckboostHwStatus;

    //config.fgetInputs = buckOpilGetMeasurements;
    config.fgetInputs = fsbuckboostHwGetMeasurements;

    //config.fapplyOutputs = buckOpilUpdateControl;
    config.fapplyOutputs = fsbuckboostHwApplyOutputs;

    config.frun = fsbuckboostControllerRun;
    config.fcontrollerInterface = fsbuckboostControllerIf;
    config.fcontrollerStatus = fsbuckboostControllerStatus;

    config.fenable = fsbuckboostHwEnable;
    //config.fenable = 0;
    config.fdisable = fsbuckboostHwDisable;
    //config.fdisable = buckOpilDisable;

    config.fonEntry = 0;
    config.fonExit = 0;

    ocpCSInitialize(FS_BUCK_BOOST_CONFIG_CS_ID, &config, "FS buck boost control");

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*----------------------------------- IRQ -----------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void fsbuckboostAdcIrq(void *callbackRef){

    uint32_t ticks;

    ticks = GetTicks();

    ocpCSRun(FS_BUCK_BOOST_CONFIG_CS_ID);
    ocpTraceSave(FS_BUCK_BOOST_CONFIG_TRACE_ID);

    ticks = ticks - GetTicks();
    texec_buck_boost = TicksToS(ticks) / 1e-6;
}
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* SOC_CPU1 */
