
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

/* Controllers */
#include "appControllerSfb.h"

#include "rp/rp.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef enum{
    APP_CONTROLLER_SFB,
    APP_CONTROLLER_END
}appControllersEnum_t;

typedef enum{
	APP_CONTROLLER_IF_SET_REF,      /* Sets the references */
	APP_CONTROLLER_IF_GET_REF,      /* Gets the references */
	APP_CONTROLLER_IF_SET,          /* Sets the active controller */
	APP_CONTROLLER_IF_GET,			/* Gets the active controller */
	APP_CONTROLLER_IF_RESET,        /* Resets the specified controller */
	APP_CONTROLLER_IF_SET_PARAMS,	/* Sets parameters for the specified controller */
	APP_CONTROLLER_IF_GET_PARAMS,	/* Gets parameters for the specified controller */
	APP_CONTROLLER_IF_END
}appControllerIfEnum_t;

typedef struct{
	rphandle_t handles[APP_CONTROLLER_IF_END];
	rpctx_t rp;
}appControllerIf_t;

typedef struct{
    appReferences_t refs;
    controller_t controller;
    controllerCallbacks_t cbs[APP_CONTROLLER_END];
    appControllerIf_t iface;
}appController_t;

//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
static void appControllerIfInit(void);

static int32_t appControllerIfSetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t appControllerIfGetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t appControllerIfSet(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t appControllerIfGet(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t appControllerIfReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t appControllerIfSetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t appControllerIfGetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
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

    config.refBuffer = (void *)&xappControler.refs;
    config.refSize = sizeof(xappControler.refs);
    
    config.getCbs = ctlGetCbs;
    config.cbsBuffer = xappControler.cbs;
    config.nControllers = APP_CONTROLLER_END;

    controllerInit(&xappControler.controller, &config);

    appControllerIfInit();

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

    return rpRequest(&xappControler.iface.rp, in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
int32_t appControllerStatus(void){

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*----------------------------- Static functions ----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static void appControllerIfInit(void){

    rpInitialize(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_END, 
        xappControler.iface.handles
    );
    
    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_SET_REF,
        appControllerIfSetReferences
    );

    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_GET_REF,
        appControllerIfGetReferences
    );

    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_SET,
        appControllerIfSet
    );

    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_GET,
        appControllerIfGet
    );

    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_RESET,
        appControllerIfReset
    );

    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_SET_PARAMS,
        appControllerIfSetParams
    );

    rpRegisterHandle(
        &xappControler.iface.rp,
        APP_CONTROLLER_IF_GET_PARAMS,
        appControllerIfGetParams
    );
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfSetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;

    status = controllerSetRef(
        &xappControler.controller,
        in, insize
    );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfGetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    void *o = (void *)*out;

    status = controllerGetRef(
        &xappControler.controller,
        o, maxoutsize
    );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfSet(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t ctl;
    ctl = *((uint32_t *)in);

    status = controllerSet(
        &xappControler.controller,
        ctl
    );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfGet(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t ctl;
    uint32_t *p = (uint32_t  *)*out;

    ctl = controllerGet(&xappControler.controller);

    *p = ctl;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t ctl;
    ctl = *((uint32_t *)in);

    status = controllerReset(&xappControler.controller, ctl);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfSetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p;
    uint32_t ctl;

    p = (uint32_t *)in;

    ctl = *p++;

    status = controllerSetParams(
        &xappControler.controller,
        ctl,
        (void *)p,
        insize - 4
    );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t appControllerIfGetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t ctl;

    ctl = *((uint32_t *)in);

    status = controllerGetParams(&xappControler.controller, ctl, *out, maxoutsize);

    return status;
}
//-----------------------------------------------------------------------------
//=============================================================================