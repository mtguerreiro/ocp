/*
 * ocpIf.c
 *
 *  Created on: 18 de fev de 2023
 *      Author: marco
 */

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "ocpIf.h"

#include "ocpTrace.h"
#include "ocpCS.h"
#include "ocpPlatform.h"
#include "ocpConfig.h"
#include "ocpOpil.h"

#include "stddef.h"
#include "string.h"
#include "stdio.h"

#include "rp/rp.h"

#ifdef OCP_CONFIG_MASTER_CORE
#include "ipc/ipcClient.h"
#endif
//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
#ifndef OCP_CONFIG_MASTER_CORE
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceRead(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetSize(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetSize(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetNumberSignals(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetSignalsNames(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetNumberTraces(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTracesNames(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetAddress(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetNumPreTrigSamples(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetNumPreTrigSamples(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetTrigSignal(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTrigSignal(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetTrigLevel(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTrigLevel(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTail(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTriggerState(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSStatus(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSDisable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSControllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSHardwareIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSGetNumberControllers(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfCSGetControllersNames(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilUpdateMeasurements(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilUpdateSimData(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilRunControl(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilInitializeControl(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilGetControl(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilGetControllerData(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
#else
static int32_t ocpIfMasterTraceRead(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSize(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetSize(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberSignals(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSignalsNames(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberTraces(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTracesNames(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetMode(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetNumPreTrigSamples(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumPreTrigSamples(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigSignal(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigSignal(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigLevel(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigLevel(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTail(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigState(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSStatus(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSEnable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSDisable(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSControllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSHardwareIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetNumberControllers(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetControllersNames(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberTracesSecondCore(void);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceResetSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSizeSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetSizeSecondCore(uint32_t id, int32_t size);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberSignalsSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSignalsNamesSecondCore(uint32_t id, char *buffer, int32_t maxsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTracesNamesSecondCore(char *buffer, int32_t maxsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetAddressSecondCore(uint32_t id, void *address);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetModeSecondCore(uint32_t id, uint32_t mode);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetModeSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetNumPreTrigSamplesSecondCore(uint32_t id, int32_t n);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumPreTrigSamplesSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigSignalSecondCore(uint32_t id, int32_t signal);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigSignalSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigLevelSecondCore(uint32_t id, float level);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigLevelSecondCore(uint32_t id, float *level);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTailSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigStateSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetNumberControllersSecondCore(void);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSStatusSecondCore(uint32_t id, int32_t *status);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSEnableSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSDisableSecondCore(uint32_t id);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetControllersNamesSecondCore(char *buffer, int32_t maxsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSControllerInterfaceSecondCore(
    uint32_t id, void *in, uint32_t insize, void **out, uint32_t maxoutsize
);
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSHardwareInterfaceSecondCore(
    uint32_t id, void *in, uint32_t insize, void **out, uint32_t maxoutsize
);
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
static int32_t ocpIfPlatformID(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
static int32_t ocpIfPlatformIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//=============================================================================

//=============================================================================
/*-------------------------------- Definitions ------------------------------*/
//=============================================================================

typedef struct{
    rphandle_t handles[OCP_IF_CMD_END];
    rpctx_t rp;
}ocpIfControl_t;

#define OCP_IF_CONFIG_DUAL_CORE_COMM_TO     60000
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static ocpIfControl_t xcontrol;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t ocpIfInitialize(void){

    rpInitialize( &xcontrol.rp, OCP_IF_CMD_END, xcontrol.handles );

#ifndef OCP_CONFIG_MASTER_CORE
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_READ, ocpIfTraceRead );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_RESET, ocpIfTraceReset );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_SIZE, ocpIfTraceGetSize );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_SIZE, ocpIfTraceSetSize );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_NUMBER_SIGNALS, ocpIfTraceGetNumberSignals );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_SIGNALS_NAMES, ocpIfTraceGetSignalsNames );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_NUMBER_TRACES, ocpIfTraceGetNumberTraces );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRACES_NAMES, ocpIfTraceGetTracesNames );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_ADDRESS, ocpIfTraceGetAddress );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_MODE, ocpIfTraceSetMode );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_MODE, ocpIfTraceGetMode );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_NUM_PRE_TRIG_SAMPLES, ocpIfTraceSetNumPreTrigSamples );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_NUM_PRE_TRIG_SAMPLES, ocpIfTraceGetNumPreTrigSamples );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_TRIG_SIGNAL, ocpIfTraceSetTrigSignal );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRIG_SIGNAL, ocpIfTraceGetTrigSignal );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_TRIG_LEVEL, ocpIfTraceSetTrigLevel );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRIG_LEVEL, ocpIfTraceGetTrigLevel );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TAIL, ocpIfTraceGetTail );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRIG_STATE, ocpIfTraceGetTriggerState );

    //rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_GET_STATUS, ocpIfCSGetStatus );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_STATUS, ocpIfCSStatus );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_ENABLE, ocpIfCSEnable );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_DISABLE, ocpIfCSDisable );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_CONTROLLER_IF, ocpIfCSControllerIf );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_HARDWARE_IF, ocpIfCSHardwareIf );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_GET_NUMBER_CONTROLLERS, ocpIfCSGetNumberControllers );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_GET_CONTROLLERS_NAMES, ocpIfCSGetControllersNames );

    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_OPIL_UPDATE_MEASUREMENTS, ocpIfOpilUpdateMeasurements );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_OPIL_UPDATE_SIM_DATA, ocpIfOpilUpdateSimData );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_OPIL_RUN_CONTROL, ocpIfOpilRunControl );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_OPIL_INITIALIZE_CONTROL, ocpIfOpilInitializeControl );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_OPIL_GET_CONTROL, ocpIfOpilGetControl );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_OPIL_GET_CONTROLLER_DATA, ocpIfOpilGetControllerData );

#else
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_READ, ocpIfMasterTraceRead );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_RESET, ocpIfMasterTraceReset );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_SIZE, ocpIfMasterTraceGetSize );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_SIZE, ocpIfMasterTraceSetSize );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_NUMBER_SIGNALS, ocpIfMasterTraceGetNumberSignals );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_SIGNALS_NAMES, ocpIfMasterTraceGetSignalsNames );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_NUMBER_TRACES, ocpIfMasterTraceGetNumberTraces );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRACES_NAMES, ocpIfMasterTraceGetTracesNames );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_MODE, ocpIfMasterTraceSetMode );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_MODE, ocpIfMasterTraceGetMode );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_NUM_PRE_TRIG_SAMPLES, ocpIfMasterTraceSetNumPreTrigSamples );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_NUM_PRE_TRIG_SAMPLES, ocpIfMasterTraceGetNumPreTrigSamples );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_TRIG_SIGNAL, ocpIfMasterTraceSetTrigSignal );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRIG_SIGNAL, ocpIfMasterTraceGetTrigSignal );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_SET_TRIG_LEVEL, ocpIfMasterTraceSetTrigLevel );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRIG_LEVEL, ocpIfMasterTraceGetTrigLevel );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TAIL, ocpIfMasterTraceGetTail );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_TRACE_GET_TRIG_STATE, ocpIfMasterTraceGetTrigState );

    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_STATUS, ocpIfMasterCSStatus );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_ENABLE, ocpIfMasterCSEnable );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_DISABLE, ocpIfMasterCSDisable );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_CONTROLLER_IF, ocpIfMasterCSControllerIf );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_HARDWARE_IF, ocpIfMasterCSHardwareIf );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_GET_NUMBER_CONTROLLERS, ocpIfMasterCSGetNumberControllers );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_CS_GET_CONTROLLERS_NAMES, ocpIfMasterCSGetControllersNames );
#endif

    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_PLATFORM_ID, ocpIfPlatformID );
    rpRegisterHandle( &xcontrol.rp, OCP_IF_CMD_PLATFORM_IF, ocpIfPlatformIf );

    return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpIf(void *in, int32_t insize, void **out, int32_t maxoutsize){

    return rpRequest( &xcontrol.rp, in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
//=============================================================================


//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
#ifndef OCP_CONFIG_MASTER_CORE
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceRead(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    size_t address;
    int32_t size;
    int32_t status;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, insize );

    status = ocpTraceGetAddress( id, (void *)(&address) );
    if( status < 0 ) return status;

    size = ocpTraceGetSize( id );
    if( size < 0 ) return size;

    if( maxoutsize < sizeof(address) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    *out = (void *)( address );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceReset(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;

    uint32_t id;
    int32_t status;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, insize );

    status = ocpTraceReset(id);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetSize(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t size;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, insize );

    size = ocpTraceGetSize(id);
    if( size < 0 ) return size;

    if( maxoutsize < sizeof(size) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&size, sizeof(size) );

    return sizeof(size);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetSize(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    uint32_t size;
    int32_t status;
    char *p;

    if( insize < ( sizeof(id) + sizeof(size) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&size, (void *)p, sizeof(size) );

    status = ocpTraceSetSize(id, size);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetNumberSignals(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t n;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, insize );

    n = ocpTraceGetNumberSignals(id);
    if( n < 0 ) return n;

    if( maxoutsize < sizeof(n) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&n, sizeof(n) );

    return sizeof(n);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetSignalsNames(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    uint32_t size;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, insize );

    size = ocpTraceGetSignalsNames(id, (char *)( *out ), maxoutsize);

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetNumberTraces(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)in;
    (void)insize;
    int32_t n;

    n = ocpTraceGetNumberTraces();
    if( n < 0 ) return n;

    if( maxoutsize < sizeof(n) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&n, sizeof(n) );

    return sizeof(n);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTracesNames(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t size;

    size = ocpTraceGetTracesNames((char *)( *out ), maxoutsize);

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetAddress(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    size_t address;
    int32_t status;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, insize );

    status = ocpTraceGetAddress( id, (void *)(&address) );
    if( status < 0 ) return status;

    if( maxoutsize < sizeof(address) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&address, sizeof(address) );

    return sizeof(size_t);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetMode(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    uint32_t mode;
    char *p;
    int32_t status;

    if( insize < ( sizeof(id) + sizeof(mode) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&mode, p, sizeof(mode) );

    status = ocpTraceSetMode(id, (ocpTraceMode_t)mode);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetMode(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t mode;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    mode = ocpTraceGetMode(id);

    if( maxoutsize < sizeof(mode) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&mode, sizeof(mode) );

    return sizeof(mode);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetNumPreTrigSamples(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    uint32_t num;
    int32_t status;
    char *p;

    if( insize < ( sizeof(id) + sizeof(num) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&num, p, sizeof(num) );

    status = ocpTraceSetNumPreTrigSamples(id, num);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetNumPreTrigSamples(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t nsamples;
    uint32_t id;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    nsamples = ocpTraceGetNumPreTrigSamples(id);

    if( maxoutsize < sizeof(nsamples) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&nsamples, sizeof(nsamples) );
    
    return sizeof(nsamples);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetTrigSignal(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    uint32_t signal;
    int32_t status;
    char *p;

    if( insize < ( sizeof(id) + sizeof(signal) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&signal, p, sizeof(signal) );

    status = ocpTraceSetTrigSignal(id, signal);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTrigSignal(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    uint32_t signal;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    signal = ocpTraceGetTrigSignal(id);

    if( maxoutsize < sizeof(signal) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&signal, sizeof(signal) );

    return sizeof(signal) ;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceSetTrigLevel(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    float level;
    int32_t status;
    char *p;

    if( insize < ( sizeof(id) + sizeof(level) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&level, p, sizeof(level) );

    status = ocpTraceSetTrigLevel(id, level);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTrigLevel(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t id;
    float level;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    status = ocpTraceGetTrigLevel(id, &level);
    if(status < 0 ) return status;

    if( maxoutsize < sizeof(level) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&level, sizeof(level) );

    return sizeof(level);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTail(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t tail;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    tail = ocpTraceGetTail(id);
    if( tail < 0 ) return tail;

    if( maxoutsize < sizeof(tail) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&tail, sizeof(tail) );

    return sizeof(tail);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfTraceGetTriggerState(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t state;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    state = ocpTraceGetTriggerState(id);
    if( state < 0 ) return state;

    if( maxoutsize < sizeof(state) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&state, sizeof(state) );

    return sizeof(state);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSStatus(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;

    int32_t status;
    int32_t cmdStatus;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    cmdStatus = ocpCSStatus(id, &status);

    if( cmdStatus < 0 ) return cmdStatus;

    if( maxoutsize < sizeof(status) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&status, sizeof(status) );

    return sizeof(status);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSEnable(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t status;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    status = ocpCSEnable(id);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSDisable(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t status;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    status = ocpCSDisable(id);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSControllerIf(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    char *p;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);

    return ocpCSControllerInterface(id, (void *)p, insize - sizeof(id), out, maxoutsize);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSHardwareIf(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    char *p;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);

    return ocpCSHardwareInterface(id, (void *)p, insize - sizeof(id), out, maxoutsize);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSGetNumberControllers(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t n;

    n = ocpCSGetNumberControllers();
    if( n < 0 ) return n;

    if( maxoutsize < sizeof(n) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&n, sizeof(n) );

    return sizeof(n);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfCSGetControllersNames(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)in;
    (void)insize;
    int32_t size;

    size = ocpCSGetControllersNames((char *)(*out), maxoutsize);

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilUpdateMeasurements(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    ocpOpilUpdateMeas(in, insize);

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilUpdateSimData(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    ocpOpilUpdateSimData(in, insize);

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilRunControl(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    ocpOpilRunControl();

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilInitializeControl(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    ocpOpilInitControl();

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilGetControl(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t size;

    size = ocpOpilGetControl(out);

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfOpilGetControllerData(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t size;

    size = ocpOpilGetControllerData(out);

    return size;
}
//-----------------------------------------------------------------------------
#else
static int32_t ocpIfMasterTraceRead(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    size_t address;
    int32_t size;
    int32_t status;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceGetAddressSecondCore( id, (void *)(&address) );
        if( status < 0 ) return status;

        size = ocpIfMasterTraceGetSizeSecondCore( id );
        if( size < 0 ) return size;
    }
    else{
        status = ocpTraceGetAddress( id, (void *)(&address) );
        if( status < 0 ) return status;

        size = ocpTraceGetSize( id );
        if( size < 0 ) return size;
    }

    *out = (void *)( address );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceReset(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t status;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceResetSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceReset(id);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSize(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t size;
    int32_t *o = (int32_t *)*out;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;
    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        size = ocpIfMasterTraceGetSizeSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        size = ocpTraceGetSize(id);
    }

    if( size < 0 ) return size;

    if( maxoutsize < sizeof(size) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&size, sizeof(size) );

    return sizeof(size);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetSize(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t size;
    int32_t status;
    char *p;
    int32_t nTracesSecondCore;

    if( insize < ( sizeof(id) + sizeof(size) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&size, (void *)p, sizeof(size) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceSetSizeSecondCore(id, size);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceSetSize(id, size);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberSignals(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t nSignals;
    int32_t *o = (int32_t *)*out;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        nSignals = ocpIfMasterTraceGetNumberSignalsSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        nSignals = ocpTraceGetNumberSignals(id);
    }

    if( nSignals < 0 ) return nSignals;

    if( maxoutsize < sizeof(nSignals) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&nSignals, sizeof(nSignals) );

    return sizeof(nSignals);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSignalsNames(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t size;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        size = ocpIfMasterTraceGetSignalsNamesSecondCore(id, (char *)( *out ), maxoutsize);
    }
    else{
        id = id - nTracesSecondCore;
        size = ocpTraceGetSignalsNames(id, (char *)( *out ), maxoutsize);
    }

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberTraces(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)in;
    (void)insize;
    int32_t nTracesSecondCore;
    int32_t nTracesThisCore;
    int32_t nTraces;

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    nTracesThisCore = ocpTraceGetNumberTraces();
    if( nTracesThisCore < 0 ) return nTracesSecondCore;

    nTraces = nTracesThisCore + nTracesSecondCore;

    if( maxoutsize < sizeof(nTraces) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&nTraces, sizeof(nTraces) );

    return sizeof(nTraces);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTracesNames(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)in;
    (void)insize;
    int32_t sizeSecondCore;
    int32_t sizeThisCore;

    char *o = (char *)( *out );

    sizeSecondCore = ocpIfMasterTraceGetTracesNamesSecondCore(o, maxoutsize);
    if( sizeSecondCore < 0 ) return sizeSecondCore;

    o = o + sizeSecondCore;
    sizeThisCore = ocpTraceGetTracesNames(o, maxoutsize - sizeSecondCore);
    if( sizeThisCore < 0 ) return sizeThisCore;

    return sizeThisCore + sizeSecondCore;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetMode(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t status;
    int32_t nTracesSecondCore;
    uint32_t mode;
    char *p;

    if( insize < ( sizeof(id) + sizeof(mode) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&mode, (void *)p, sizeof(mode) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceSetModeSecondCore(id, mode);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceSetMode(id, mode);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetMode(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t nTracesSecondCore;
    int32_t mode;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        mode = ocpIfMasterTraceGetModeSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        mode = ocpTraceGetMode(id);
    }

    if( maxoutsize < sizeof(mode) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&mode, sizeof(mode) );

    return sizeof(mode);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetNumPreTrigSamples(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t num;
    int32_t status;
    char *p;
    int32_t nTracesSecondCore;

    if( insize < ( sizeof(id) + sizeof(num) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&num, (void *)p, sizeof(num) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceSetNumPreTrigSamplesSecondCore(id, num);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceSetNumPreTrigSamples(id, num);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumPreTrigSamples(void *in, uint32_t insize,
        void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t num;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        num = ocpIfMasterTraceGetNumPreTrigSamplesSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        num = ocpTraceGetNumPreTrigSamples(id);
    }
    if( num < 0 ) return num;

    if( maxoutsize < sizeof(num) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&num, sizeof(num) );

    return sizeof(num);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigSignal(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t trace;
    int32_t status;
    char *p;
    int32_t nTracesSecondCore;

    if( insize < ( sizeof(id) + sizeof(trace) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&trace, (void *)p, sizeof(trace) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceSetTrigSignalSecondCore(id, trace);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceSetTrigSignal(id, trace);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigSignal(void *in, uint32_t insize,
        void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t signal;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        signal = ocpIfMasterTraceGetTrigSignalSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        signal = ocpTraceGetTrigSignal(id);
    }
    if( signal < 0 ) return signal;

    if( maxoutsize < sizeof(signal) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&signal, sizeof(signal) );

    return sizeof(signal);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigLevel(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    float level;
    int32_t status;
    char *p;
    int32_t nTracesSecondCore;

    if( insize < ( sizeof(id) + sizeof(level) ) )
        return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);
    memcpy( (void *)&level, (void *)p, sizeof(level) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceSetTrigLevelSecondCore(id, level);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceSetTrigLevel(id, level);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigLevel(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    float level;
    int32_t status;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        status = ocpIfMasterTraceGetTrigLevelSecondCore(id, &level);
    }
    else{
        id = id - nTracesSecondCore;
        status = ocpTraceGetTrigLevel(id, &level);
    }
    if( status < 0 ) return status;

    if( maxoutsize < sizeof(level) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&level, sizeof(level) );

    return sizeof(level);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTail(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t tail;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        tail = ocpIfMasterTraceGetTailSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        tail = ocpTraceGetTail(id);
    }
    if( tail < 0 ) return tail;

    if( maxoutsize < sizeof(tail) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&tail, sizeof(tail) );

    return sizeof(tail);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigState(void *in, uint32_t insize,
        void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t state;
    int32_t nTracesSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nTracesSecondCore = ocpIfMasterTraceGetNumberTracesSecondCore();
    if( nTracesSecondCore < 0 ) return nTracesSecondCore;

    if( id < (uint32_t) nTracesSecondCore ){
        state = ocpIfMasterTraceGetTrigStateSecondCore(id);
    }
    else{
        id = id - nTracesSecondCore;
        state = ocpTraceGetTriggerState(id);
    }

    if( state < 0 ) return state;

    if( maxoutsize < sizeof(state) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&state, sizeof(state) );

    return sizeof(state);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSStatus(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t id;
    int32_t status;
    int32_t cmdStatus;
    int32_t nControllersSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nControllersSecondCore = ocpIfMasterCSGetNumberControllersSecondCore();
    if( nControllersSecondCore < 0 ) return nControllersSecondCore;

    if( id < (uint32_t) nControllersSecondCore ) {
        cmdStatus = ocpIfMasterCSStatusSecondCore(id, &status);
    }
    else{
        id = id - nControllersSecondCore;
        cmdStatus = ocpCSStatus(id, &status);
    }

    if( cmdStatus < 0 ) return cmdStatus;

    if( maxoutsize < sizeof(cmdStatus) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&cmdStatus, sizeof(cmdStatus) );

    return sizeof(cmdStatus);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSEnable(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t status;
    int32_t nControllersSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nControllersSecondCore = ocpIfMasterCSGetNumberControllersSecondCore();
    if( nControllersSecondCore < 0 ) return nControllersSecondCore;

    if( id < (uint32_t) nControllersSecondCore ) {
        status = ocpIfMasterCSEnableSecondCore(id);
    }
    else{
        id = id - nControllersSecondCore;
        status = ocpCSEnable(id);
    }

    if( status < 0 ) return status;

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSDisable(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    uint32_t id;
    int32_t status;
    int32_t nControllersSecondCore;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    nControllersSecondCore = ocpIfMasterCSGetNumberControllersSecondCore();
    if( nControllersSecondCore < 0 ) return nControllersSecondCore;

    if( id < (uint32_t) nControllersSecondCore ) {
        status = ocpIfMasterCSDisableSecondCore(id);
    }
    else{
        id = id - nControllersSecondCore;
        status = ocpCSDisable(id);
    }

    if( status < 0 ) return status;

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSControllerIf(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    int32_t nControllersSecondCore;
    uint32_t id;
    char *p;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);

    nControllersSecondCore = ocpIfMasterCSGetNumberControllersSecondCore();
    if( nControllersSecondCore < 0 ) return nControllersSecondCore;

    if( id < (uint32_t) nControllersSecondCore ) {
        status = ocpIfMasterCSControllerInterfaceSecondCore(id, (void *)p, insize - sizeof(id), out, maxoutsize);
    }
    else{
        id = id - nControllersSecondCore;
        status = ocpCSControllerInterface(id, (void *)p, insize - sizeof(id), out, maxoutsize);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSHardwareIf(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    int32_t nControllersSecondCore;
    uint32_t id;
    char *p;

    if( insize < sizeof(id) ) return OCP_IF_ERR_INVALID_IN_SIZE;

    memcpy( (void *)&id, in, sizeof(id) );

    p = (char *)in + sizeof(id);

    nControllersSecondCore = ocpIfMasterCSGetNumberControllersSecondCore();
    if( nControllersSecondCore < 0 ) return nControllersSecondCore;

    if( id < (uint32_t) nControllersSecondCore ) {
        status = ocpIfMasterCSHardwareInterfaceSecondCore(id, (void *)p, insize - sizeof(id), out, maxoutsize);
    }
    else{
        id = id - nControllersSecondCore;
        status = ocpCSHardwareInterface(id, (void *)p, insize - sizeof(id), out, maxoutsize);
    }

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetNumberControllers(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)in;
    (void)insize;
    int32_t nControllersSecondCore, nControllersThisCore;
    int32_t nControllers;

    nControllersSecondCore = ocpIfMasterCSGetNumberControllersSecondCore();
    if( nControllersSecondCore < 0 ) return nControllersSecondCore;

    nControllersThisCore = ocpCSGetNumberControllers();
    if( nControllersThisCore < 0 ) return nControllersThisCore;

    nControllers = nControllersThisCore + nControllersSecondCore;

    if( maxoutsize < sizeof(nControllers) ) return OCP_IF_ERR_INVALID_OUT_SIZE;
    memcpy( *out, (void *)&nControllers, sizeof(nControllers) );

    return sizeof(nControllers);
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetControllersNames(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t sizeSecondCore;
    int32_t sizeThisCore;

    char *o = (char *)( *out );

    sizeSecondCore = ocpIfMasterCSGetControllersNamesSecondCore(o, maxoutsize);
    if( sizeSecondCore < 0 ) return sizeSecondCore;

    o = o + sizeSecondCore;
    sizeThisCore = ocpCSGetControllersNames(o, maxoutsize - sizeSecondCore);
    if( sizeThisCore < 0 ) return sizeThisCore;

    return sizeThisCore + sizeSecondCore;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberTracesSecondCore(void){

    int32_t status;

    uint32_t nTracesSecondCore;
    uint32_t *p;
    uint32_t cmd;

    cmd = OCP_IF_CMD_TRACE_GET_NUMBER_TRACES;
    p = &nTracesSecondCore;
    status = ipcClientRequest(
        (void *)&cmd, sizeof(cmd),
        (void **)&p, sizeof(nTracesSecondCore),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return nTracesSecondCore;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceResetSecondCore(uint32_t id){

    int32_t status;
    uint32_t cmd[2];

    cmd[0] = OCP_IF_CMD_TRACE_RESET;
    cmd[1] = id;

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSizeSecondCore(uint32_t id){

    int32_t size;
    int32_t status;
    int32_t *p;
    uint32_t cmd[2];

    p = &size;

    cmd[0] = OCP_IF_CMD_TRACE_GET_SIZE;
    cmd[1] = id;

    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(size),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetSizeSecondCore(uint32_t id, int32_t size){

    int32_t status;
    uint32_t cmd[3];

    cmd[0] = OCP_IF_CMD_TRACE_SET_SIZE;
    cmd[1] = id;
    cmd[2] = size;
    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumberSignalsSecondCore(uint32_t id){

    int32_t status;
    int32_t nSignals;
    uint32_t cmd[2];
    int32_t *p;

    cmd[0] = OCP_IF_CMD_TRACE_GET_NUMBER_SIGNALS;
    cmd[1] = id;

    p = &nSignals;
    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(nSignals),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return nSignals;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetSignalsNamesSecondCore(
    uint32_t id, char *buffer, int32_t maxsize){

    int32_t size;
    uint32_t cmd[2];

    cmd[0] = OCP_IF_CMD_TRACE_GET_SIGNALS_NAMES;
    cmd[1] = id;

    size = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&buffer, maxsize,
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTracesNamesSecondCore(char *buffer, int32_t maxsize){

    int32_t size;
    uint32_t cmd;

    cmd = OCP_IF_CMD_TRACE_GET_TRACES_NAMES;

    size = ipcClientRequest(
        (void *)&cmd, sizeof(cmd),
        (void **)&buffer, maxsize,
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetAddressSecondCore(uint32_t id, void *address){

    int32_t size;
    uint32_t cmd[2];

    cmd[0] = OCP_IF_CMD_TRACE_GET_ADDRESS;
    cmd[1] = id;

    size = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&address, sizeof(void *),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetModeSecondCore(uint32_t id, uint32_t mode){

    int32_t status;
    uint32_t cmd[3];

    cmd[0] = OCP_IF_CMD_TRACE_SET_MODE;
    cmd[1] = id;
    cmd[2] = mode;

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetModeSecondCore(uint32_t id){

    int32_t status;
    uint32_t cmd[2];
    int32_t mode;
    int32_t *p;

    cmd[0] = OCP_IF_CMD_TRACE_GET_MODE;
    cmd[1] = id;

    p = &mode;
    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(mode),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return mode;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetNumPreTrigSamplesSecondCore(uint32_t id, int32_t n){

    int32_t status;
    uint32_t cmd[3];

    cmd[0] = OCP_IF_CMD_TRACE_SET_NUM_PRE_TRIG_SAMPLES;
    cmd[1] = id;
    cmd[2] = n;

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetNumPreTrigSamplesSecondCore(uint32_t id){

    int32_t status;
    uint32_t cmd[2];
    int32_t n;
    int32_t *p;

    cmd[0] = OCP_IF_CMD_TRACE_GET_NUM_PRE_TRIG_SAMPLES;
    cmd[1] = id;

    p = &n;
    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(n),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );

    if(status < 0 ) return status;

    return n;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigSignalSecondCore(uint32_t id, int32_t signal){

    int32_t status;
    uint32_t cmd[3];

    cmd[0] = OCP_IF_CMD_TRACE_SET_TRIG_SIGNAL;
    cmd[1] = id;
    cmd[2] = signal;

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigSignalSecondCore(uint32_t id){

    int32_t signal;
    int32_t status;
    uint32_t cmd[2];
    int32_t *p;

    cmd[0] = OCP_IF_CMD_TRACE_GET_TRIG_SIGNAL;
    cmd[1] = id;

    p = &signal;
    status = ipcClientRequest(
        (void *)cmd, 8,
        (void **)&p, sizeof(signal),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return signal;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceSetTrigLevelSecondCore(uint32_t id, float level){

    int32_t status;
    uint32_t cmd[3];

    cmd[0] = OCP_IF_CMD_TRACE_SET_TRIG_LEVEL;
    cmd[1] = id;
    memcpy( (void *)&cmd[2], (void *)&level,  sizeof(float) );

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigLevelSecondCore(uint32_t id, float *level){

    int32_t status;
    uint32_t cmd[3];
    float lvl;
    float *p;

    cmd[0] = OCP_IF_CMD_TRACE_GET_TRIG_LEVEL;
    cmd[1] = id;

    p = &lvl;
    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(lvl),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    *level = lvl;

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTailSecondCore(uint32_t id){

    int32_t tail;
    int32_t status;
    int32_t *p;
    uint32_t cmd[2];

    p = &tail;

    cmd[0] = OCP_IF_CMD_TRACE_GET_TAIL;
    cmd[1] = id;

    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(tail),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return tail;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterTraceGetTrigStateSecondCore(uint32_t id){

    int32_t state;
    int32_t status;
    int32_t *p;
    uint32_t cmd[2];

    p = &state;

    cmd[0] = OCP_IF_CMD_TRACE_GET_TRIG_STATE;
    cmd[1] = id;

    status = ipcClientRequest(
        (void *)cmd, sizeof(cmd),
        (void **)&p, sizeof(state),
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );
    if( status < 0 ) return status;

    return state;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetNumberControllersSecondCore(void){

    int32_t status;

    uint32_t nControllersSecondCore;
    uint32_t *p;
    uint32_t cmd;

    cmd = OCP_IF_CMD_CS_GET_NUMBER_CONTROLLERS;
    p = &nControllersSecondCore;
    status = ipcClientRequest( (void *)&cmd, 4, (void **)&p, 4, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );
    if( status < 0 ) return status;

    return nControllersSecondCore;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSStatusSecondCore(uint32_t id, int32_t *status){

    int32_t cmdStatus;
    uint32_t cmd[2];

    cmd[0] = OCP_IF_CMD_CS_STATUS;
    cmd[1] = id;

    cmdStatus = ipcClientRequest( (void *)cmd, sizeof(cmd), (void **)&status, sizeof(void *), OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return cmdStatus;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSEnableSecondCore(uint32_t id){

    int32_t status;
    uint32_t cmd[2];

    cmd[0] = OCP_IF_CMD_CS_ENABLE;
    cmd[1] = id;

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSDisableSecondCore(uint32_t id){

    int32_t status;
    uint32_t cmd[2];

    cmd[0] = OCP_IF_CMD_CS_DISABLE;
    cmd[1] = id;

    status = ipcClientRequest( (void *)cmd, sizeof(cmd), 0, 0, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSGetControllersNamesSecondCore(char *buffer, int32_t maxsize){

    int32_t size;
    uint32_t cmd;

    cmd = OCP_IF_CMD_CS_GET_CONTROLLERS_NAMES;

    size = ipcClientRequest( (void *)&cmd, sizeof(cmd), (void **)&buffer, maxsize, OCP_IF_CONFIG_DUAL_CORE_COMM_TO );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSControllerInterfaceSecondCore(
    uint32_t id, void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t d;
    int32_t size;
    uint32_t k;
    char buffer[insize + sizeof(d) + sizeof(id)];

    d = OCP_IF_CMD_CS_CONTROLLER_IF;
    memcpy( (void *)buffer, (void *)&d, sizeof(d) );

    memcpy( (void *)&buffer[sizeof(d)], (void *)&id, sizeof(id) );

    memcpy( (void *) &buffer[ sizeof(d) + sizeof(id) ], in, insize );

    size = ipcClientRequest(
        (void *)buffer, insize + sizeof(d) + sizeof(id),
        out, maxoutsize,
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfMasterCSHardwareInterfaceSecondCore(
    uint32_t id, void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t d;
    int32_t size;
    uint32_t k;
    char buffer[insize + sizeof(d) + sizeof(id)];

    d = OCP_IF_CMD_CS_HARDWARE_IF;
    memcpy( (void *)buffer, (void *)&d, sizeof(d) );

    memcpy( (void *)&buffer[sizeof(d)], (void *)&id, sizeof(id) );

    memcpy( (void *) &buffer[ sizeof(d) + sizeof(id) ], in, insize );

    size = ipcClientRequest(
        (void *)buffer, insize + sizeof(d) + sizeof(id),
        out, maxoutsize,
        OCP_IF_CONFIG_DUAL_CORE_COMM_TO
    );

    return size;
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
static int32_t ocpIfPlatformID(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t size;

    char *o = (char *)( *out );

    size = ocpPlatformID(o, maxoutsize);

    return size;
}
//-----------------------------------------------------------------------------
static int32_t ocpIfPlatformIf(
    void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    return ocpPlatformIf(in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
//=============================================================================
