
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "appController.h"

#include "appConfig.h"

/* OCP */
#include "ocp/ocpTrace.h"
#include "rp/rp.h"

/* Controller lib */
#include "controller/controller.h"
#include "controller/controllerIf.h"

/* Controllers */
#include "appControllerSfb.h"
#include "appControllerCascaded.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef enum{
    APP_CONTROLLER_SFB,
    APP_CONTROLLER_CASCADED,
    APP_CONTROLLER_END
}appControllersEnum_t;

typedef struct{
    appReferences_t refs;
    controller_t controller;
    controllerCallbacks_t cbs[APP_CONTROLLER_END];
}appController_t;

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
appController_t xappControler;
//============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t appControllerInit(void){

    controllerConfig_t config;

    controllerGetCbs_t ctlGetCbs[APP_CONTROLLER_END] = {0};
    ctlGetCbs[APP_CONTROLLER_SFB] = appControlSfbGetCallbacks;
    ctlGetCbs[APP_CONTROLLER_CASCADED] = appControlCascadedGetCallbacks;

    config.refBuffer = (void *)&xappControler.refs;
    config.refSize = sizeof(xappControler.refs);
    
    config.getCbs = ctlGetCbs;
    config.cbsBuffer = xappControler.cbs;
    config.nControllers = APP_CONTROLLER_END;

    controllerInit(&xappControler.controller, &config);

    controllerIfRegister(&xappControler.controller, APP_CONFIG_OCP_CS_ID);

    return 0;
}
//-----------------------------------------------------------------------------
int32_t appControllerRun(void *inputs, int32_t ninputs, void *outputs, int32_t nmaxoutputs){
    
    int32_t status;

    status = controllerRun(
        &xappControler.controller,
        inputs, ninputs,
        outputs, nmaxoutputs);

    return status;
}
//-----------------------------------------------------------------------------
int32_t appControllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    return controllerIf(in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
int32_t appControllerStatus(void){

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================