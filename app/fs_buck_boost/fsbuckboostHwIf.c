/*
 * fsbuckboostHwIf.c
 *
 */
#ifdef SOC_CPU1
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "fsbuckboostHwIf.h"

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
#include "fsbuckboostHwOpil.h"
#else
#include "fsbuckboostHw.h"
#endif

#include "fsbuckboostConfig.h"

#include "rp.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{
    rphandle_t handles[FS_BUCK_BOOST_HW_IF_END];
    rpctx_t rp;
}fsbuckboostHwIf_t;

typedef struct{

    fsbuckboostHwIf_t interface;
}fsbuckboostHwIfControl_t;

static fsbuckboostHwIfControl_t hwControl;
//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
static int32_t fsbuckboostHwIfSetPwmReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmOutputEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmOutputEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmOvfTriggerEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmOvfTriggerEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmInv(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmInv(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmLsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmLsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmHsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmHsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmFrequency(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmFrequency(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmDutyCycle(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmDutyCycle(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetPwmDeadTime(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetPwmDeadTime(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetAdcEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetAdcEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetAdcManualTrig(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetAdcManualTrig(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetAdcInterruptEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetAdcInterruptEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetAdcSpiFreq(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetAdcSpiFreq(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetOutputRelay(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetOutputRelay(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetLoadSwitch(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetLoadSwitch(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfSetMeasGains(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetMeasGains(void *in, uint32_t insize, void **out, uint32_t maxoutsize);

static int32_t fsbuckboostHwIfClearStatus(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t fsbuckboostHwIfGetStatus(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
int32_t fsbuckboostHwIfInitialize(void){

    /* Initializes the request processor */
    rpInitialize(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_END, hwControl.interface.handles);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_RESET, fsbuckboostHwIfSetPwmReset);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_RESET, fsbuckboostHwIfGetPwmReset);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_OUTPUT_ENABLE, fsbuckboostHwIfSetPwmOutputEnable);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_OUTPUT_ENABLE, fsbuckboostHwIfGetPwmOutputEnable);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_OVF_TRIGGER_ENABLE, fsbuckboostHwIfSetPwmOvfTriggerEnable);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_OVF_TRIGGER_ENABLE, fsbuckboostHwIfGetPwmOvfTriggerEnable);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_INV, fsbuckboostHwIfSetPwmInv);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_INV, fsbuckboostHwIfGetPwmInv);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_MODE, fsbuckboostHwIfSetPwmMode);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_MODE, fsbuckboostHwIfGetPwmMode);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_LS_SW, fsbuckboostHwIfSetPwmLsSw);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_LS_SW, fsbuckboostHwIfGetPwmLsSw);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_HS_SW, fsbuckboostHwIfSetPwmHsSw);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_HS_SW, fsbuckboostHwIfGetPwmHsSw);
    
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_FREQ, fsbuckboostHwIfSetPwmFrequency);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_FREQ, fsbuckboostHwIfGetPwmFrequency);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_DUTY, fsbuckboostHwIfSetPwmDutyCycle);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_DUTY, fsbuckboostHwIfGetPwmDutyCycle);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_PWM_DEAD_TIME, fsbuckboostHwIfSetPwmDeadTime);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_PWM_DEAD_TIME, fsbuckboostHwIfGetPwmDeadTime);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_ADC_ENABLE, fsbuckboostHwIfSetAdcEnable);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_ADC_ENABLE, fsbuckboostHwIfGetAdcEnable);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_ADC_MANUAL_TRIG, fsbuckboostHwIfSetAdcManualTrig);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_ADC_MANUAL_TRIG, fsbuckboostHwIfGetAdcManualTrig);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_ADC_INT_ENABLE, fsbuckboostHwIfSetAdcInterruptEnable);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_ADC_INT_ENABLE, fsbuckboostHwIfGetAdcInterruptEnable);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_ADC_SPI_FREQ, fsbuckboostHwIfSetAdcSpiFreq);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_ADC_SPI_FREQ, fsbuckboostHwIfGetAdcSpiFreq);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_OUTPUT_RELAY, fsbuckboostHwIfSetOutputRelay);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_OUTPUT_RELAY, fsbuckboostHwIfGetOutputRelay);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_LOAD_SW, fsbuckboostHwIfSetLoadSwitch);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_LOAD_SW, fsbuckboostHwIfGetLoadSwitch);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_SET_MEAS_GAINS, fsbuckboostHwIfSetMeasGains);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_MEAS_GAINS, fsbuckboostHwIfGetMeasGains);

    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_CLEAR_STATUS, fsbuckboostHwIfClearStatus);
    rpRegisterHandle(&hwControl.interface.rp, FS_BUCK_BOOST_HW_IF_GET_STATUS, fsbuckboostHwIfGetStatus);

    return 0;
}
//-----------------------------------------------------------------------------
int32_t fsbuckboostHwIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;

    status = rpRequest(&hwControl.interface.rp, in, insize, out, maxoutsize);

    return status;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*----------------------------- Static functions ----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t reset;

    reset = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmReset(reset);
#else
    fsbuckboostHwSetPwmReset(reset);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t reset;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    reset = fsbuckboostHwOpilGetPwmReset();
#else
    reset = fsbuckboostHwGetPwmReset();
#endif

    *o = reset;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmOutputEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t enable;

    enable = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmOutputEnable(enable);
#else
    fsbuckboostHwSetPwmOutputEnable(enable);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmOutputEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t enable;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    enable = fsbuckboostHwOpilGetPwmOutputEnable();
#else
    enable = fsbuckboostHwGetPwmOutputEnable();
#endif

    *o = enable;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmOvfTriggerEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t enable;

    enable = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmOvfTriggerEnable(enable);
#else
    fsbuckboostHwSetPwmOvfTriggerEnable(enable);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmOvfTriggerEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t enable;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    enable = fsbuckboostHwOpilGetPwmOvfTriggerEnable();
#else
    enable = fsbuckboostHwGetPwmOvfTriggerEnable();
#endif

    *o = enable;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmInv(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t enable;

    enable = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmInv(enable);
#else
    fsbuckboostHwSetPwmInv(enable);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmInv(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t enable;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    enable = fsbuckboostHwOpilGetPwmInv();
#else
    enable = fsbuckboostHwGetPwmInv();
#endif

    *o = enable;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t mode;

    mode = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmMode(mode);
#else
    fsbuckboostHwSetPwmMode(mode);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t mode;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    mode = fsbuckboostHwOpilGetPwmMode();
#else
    mode = fsbuckboostHwGetPwmMode();
#endif

    *o = mode;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmLsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t state;

    state = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmLsSw(state);
#else
    fsbuckboostHwSetPwmLsSw(state);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmLsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t state;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    state = fsbuckboostHwOpilGetPwmLsSw();
#else
    state = fsbuckboostHwGetPwmLsSw();
#endif

    *o = state;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmHsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t state;

    state = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmHsSw(state);
#else
    fsbuckboostHwSetPwmHsSw(state);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmHsSw(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t state;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    state = fsbuckboostHwOpilGetPwmHsSw();
#else
    state = fsbuckboostHwGetPwmHsSw();
#endif

    *o = state;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmFrequency(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t freq;

    freq = *( (uint32_t *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmFrequency(freq);
#else
    fsbuckboostHwSetPwmFrequency(freq);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmFrequency(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t freq;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    freq = fsbuckboostHwOpilGetPwmFrequency();
#else
    freq = fsbuckboostHwGetPwmFrequency();
#endif

    *o = freq;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmDutyCycle(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    float duty;

    duty = *( (float *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmDuty(duty);
#else
    fsbuckboostHwSetPwmDuty(duty);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmDutyCycle(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    float *o = (float *)*out;
    float duty;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    duty = fsbuckboostHwOpilGetPwmDuty();
#else
    duty = fsbuckboostHwGetPwmDuty();
#endif

    *o = duty;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetPwmDeadTime(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    float deadtime;

    deadtime = *( (float *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetPwmDeadTime(deadtime);
#else
    fsbuckboostHwSetPwmDeadTime(deadtime);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetPwmDeadTime(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    float *o = (float *)*out;
    float deadtime;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    deadtime = fsbuckboostHwOpilGetPwmDeadTime();
#else
    deadtime = fsbuckboostHwGetPwmDeadTime();
#endif

    *o = deadtime;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetAdcEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t enable;

    enable = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetAdcEnable(enable);
#else
    fsbuckboostHwSetAdcEnable(enable);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetAdcEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t enable;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    enable = fsbuckboostHwOpilGetAdcEnable();
#else
    enable = fsbuckboostHwGetAdcEnable();
#endif

    *o = enable;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetAdcManualTrig(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t trigger;

    trigger = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetAdcManualTrigger(trigger);
#else
    fsbuckboostHwSetAdcManualTrigger(trigger);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetAdcManualTrig(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t trigger;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    trigger = fsbuckboostHwOpilGetAdcManualTrigger();
#else
    trigger = fsbuckboostHwGetAdcManualTrigger();
#endif

    *o = trigger;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetAdcInterruptEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t enable;

    enable = *( (uint32_t *)in ) & 0x01;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetAdcInterruptEnable(enable);
#else
    fsbuckboostHwSetAdcInterruptEnable(enable);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetAdcInterruptEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t enable;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    enable = fsbuckboostHwOpilGetAdcInterruptEnable();
#else
    enable = fsbuckboostHwGetAdcInterruptEnable();
#endif

    *o = enable;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetAdcSpiFreq(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t freq;

    freq = *( (uint32_t *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetAdcSpiFreq(freq);
#else
    fsbuckboostHwSetAdcSpiFreq(freq);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetAdcSpiFreq(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t freq;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    freq = fsbuckboostHwOpilGetAdcSpiFreq();
#else
    freq = fsbuckboostHwGetAdcSpiFreq();
#endif

    *o = freq;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetOutputRelay(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t state;

    state = *( (uint32_t *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetOutputRelay(state);
#else
    fsbuckboostHwSetOutputRelay(state);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetOutputRelay(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t state;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    state = fsbuckboostHwOpilGetOutputRelay();
#else
    state = fsbuckboostHwGetOutputRelay();
#endif

    *o = state;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetLoadSwitch(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t state;

    state = *( (uint32_t *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetLoadSwitch(state);
#else
    fsbuckboostHwSetLoadSwitch(state);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetLoadSwitch(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t *o = (uint32_t *)*out;
    uint32_t state;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    state = fsbuckboostHwOpilGetLoadSwitch();
#else
    state = fsbuckboostHwGetLoadSwitch();
#endif

    *o = state;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfSetMeasGains(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    fsbuckboostConfigMeasGains_t *gains;

    gains = ( (fsbuckboostConfigMeasGains_t *)in );

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilSetMeasGains(gains);
#else
    fsbuckboostHwSetMeasGains(gains);
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetMeasGains(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    fsbuckboostConfigMeasGains_t *o = (fsbuckboostConfigMeasGains_t *)*out;
    fsbuckboostConfigMeasGains_t gains;
    uint32_t size;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    size = fsbuckboostHwOpilGetMeasGains(&gains);
#else
    size = fsbuckboostHwGetMeasGains(&gains);
#endif

    *o = gains;

    return size;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfClearStatus(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    fsbuckboostHwOpilStatusClear();
#else
    fsbuckboostHwStatusClear();
#endif

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t fsbuckboostHwIfGetStatus(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t status;
    uint32_t *o = (uint32_t *)*out;

#ifdef FS_BUCK_BOOST_HW_IF_CONFIG_OPIL
    status = fsbuckboostHwOpilStatus();
#else
    status = fsbuckboostHwStatus();
#endif

    *o = status;

    return 4;
}
//-----------------------------------------------------------------------------
//=============================================================================
#endif /* SOC_CPU1 */
