
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "ocpOpilThread.h"

#include "stdlib.h"
#include "stdio.h"

#include "opiltarget.h"

#include "comm/targetCommSock.h"

#include "ocp/ocpIf.h"

#include "stypes.h"

//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t opilOcpUpdateMeasurements(void *meas, int32_t size);
static int32_t opilOcpUpdateSimData(void *simData, int32_t size);
void opilOcpRunControl(void);
static int32_t opilOcpGetControl(void **control);
static int32_t opilOcpGetControllerData(void **controllerData);
static void opilOcpInitControl(void);
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static uint8_t txbuf[256];
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void *ocpOpilThread(void *ptr){

    (void)ptr;
    int32_t conn;

    /* Initializes opil */
    opiltargetCommConfig_t comm;
    opiltargetControlConfig_t control;

    comm.openConn = targetCommSockOpenConnection;
    comm.closeConn = targetCommSockCloseConnection;
    comm.sendData = targetCommSockSendData;
    comm.receiveData = targetCommSockReceiveData;

    control.updateMeas = opilOcpUpdateMeasurements;
    control.updateSimData = opilOcpUpdateSimData;

    control.initControl = opilOcpInitControl;
    control.runControl = opilOcpRunControl;

    control.getControl = opilOcpGetControl;
    control.getControllerData = opilOcpGetControllerData;

    opiltargetInitialize(&comm, &control);

    /* Runs opil target */
    LogInfo( ("OPiL target module running") );

    while( 1 ){
        LogInfo( ("Waiting for a connection") );
        conn = opiltargetConnectToHost(0);

        if( conn != 0 ){
            LogError( ("Error receiving a connection, trying it again") );
            continue;
        }

        LogInfo( ("Connection received! Running controller...") );

        while( opiltargetExchangeDataHost() == 0 );

        LogInfo( ("Connection terminated. Starting over") );
        opiltargetDisconnectFromHost(0);
    }

    LogInfo( ("Closing OPiL target module") );

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static void opilOcpInitControl(void){

}
//-----------------------------------------------------------------------------
static int32_t opilOcpUpdateMeasurements(void *meas, int32_t size){

    int32_t status;
    int32_t *p;
    int32_t k;

    uint8_t *dst, *src;

    k = size;
    p = (int32_t *)txbuf;
    *p = OCP_IF_CMD_OPIL_UPDATE_MEASUREMENTS;

    dst = &txbuf[4];
    src = (uint8_t *)meas;
    for(k = 0; k < size; k++) *dst++ = *src++;

    status = ocpIf( (void *)&txbuf, size + 4, 0, 0 );
    if( status < 0 ) return status;

    return 0;
}
//-----------------------------------------------------------------------------
static int32_t opilOcpUpdateSimData(void *simData, int32_t size){

    int32_t status;
    int32_t *p;
    int32_t k;

    uint8_t *dst, *src;

    k = size;
    p = (int32_t *)txbuf;
    *p = OCP_IF_CMD_OPIL_UPDATE_SIM_DATA;

    dst = &txbuf[4];
    src = (uint8_t *)simData;
    for(k = 0; k < size; k++) *dst++ = *src++;

    status = ocpIf( (void *)&txbuf, size + 4, 0, 0 );
    if( status < 0 ) return status;

    return 0;
}
//-----------------------------------------------------------------------------
void opilOcpRunControl(void){

    int32_t *p;

    p = (int32_t *)txbuf;
    *p = OCP_IF_CMD_OPIL_RUN_CONTROL;

    ocpIf( (void *)&txbuf, 4, 0, 0 );
}
//-----------------------------------------------------------------------------
static int32_t opilOcpGetControl(void **control){

    int32_t size;
    int32_t *p;

    p = (int32_t *)txbuf;
    *p = OCP_IF_CMD_OPIL_GET_CONTROL;

    size = ocpIf( (void *)&txbuf, 4, control, sizeof(stypesControl_t) );

    return size;
}
//-----------------------------------------------------------------------------
static int32_t opilOcpGetControllerData(void **controllerData){

    int32_t size;
    int32_t *p;

    p = (int32_t *)txbuf;
    *p = OCP_IF_CMD_OPIL_GET_CONTROLLER_DATA;

    size = ocpIf( (void *)&txbuf, 4, controllerData, sizeof(stypesControllerData_t) );

    return size;
}
//-----------------------------------------------------------------------------
//=============================================================================
