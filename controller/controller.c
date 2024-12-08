
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "controller.h"

//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
static int32_t controllerSwitch(controller_t *controller,
    void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs);
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void controllerInit(controller_t *controller, controllerConfig_t *config){

    uint32_t k;

    controller->refs.buffer = config->refBuffer;
    controller->refs.size = config->refSize;

    controller->cbs = config->cbsBuffer;
    controller->nControllers = config->nControllers;

    /* 
     * Initializing active as nControllers ensures controllerRun does not run
     * until a valid controller is set. 
     */
    controller->active = config->nControllers;
    controller->previous = config->nControllers;

    for(k = 0; k < config->nControllers; k++){
        config->getCbs[k]( (void *) &controller->cbs[k] );
    }

    for(k = 0; k < config->nControllers; k++){
        if( controller->cbs[k].init != 0 )
            controller->cbs[k].init();
    }
}
//-----------------------------------------------------------------------------
int32_t controllerRun(controller_t *controller,
    void *meas, int32_t nmeas, 
    void *outputs, int32_t nmaxoutputs){
    
    int32_t status;
    uint32_t active = controller->active;
    uint32_t previous = controller->previous;

    if( active >= controller->nControllers ) return CONTROLLER_ERR_INVALID_ID;

    if( active != previous ){
        status = controllerSwitch(
            controller, meas, nmeas,
            controller->refs.buffer, controller->refs.size,
            outputs, nmaxoutputs);
        if( status < 0 ) return status;
    }

    status = controller->cbs[active].run(
        meas, nmeas,
        controller->refs.buffer, controller->refs.size, 
        outputs, nmaxoutputs
    );

    return status;
}
//-----------------------------------------------------------------------------
int32_t controllerSetRef(controller_t *controller, void *ref, uint32_t size){

    uint32_t k;
    uint8_t *src = (uint8_t *)ref;
    uint8_t *dst = (uint8_t *)controller->refs.buffer;

    if( size != controller->refs.size ) return CONTROLLER_ERR_INVALID_REF_SIZE;

    for(k = 0; k < size; k++){
        *dst++ = *src++;
    }

    return 0;
}
//-----------------------------------------------------------------------------
int32_t controllerGetRef(controller_t *controller, void *buffer, uint32_t size){

    uint32_t k;
    uint8_t *src = (uint8_t *)controller->refs.buffer;
    uint8_t *dst = (uint8_t *)buffer;

    if( size < controller->refs.size ) return CONTROLLER_ERR_INVALID_REF_BUF_SIZE;

    for(k = 0; k < controller->refs.size; k++){
        *dst++ = *src++;
    }

    return controller->refs.size;
}
//-----------------------------------------------------------------------------
int32_t controllerSet(controller_t *controller, uint32_t id){

    if( id >= controller->nControllers ) return CONTROLLER_ERR_INVALID_ID;

    controller->active = id;

    return 0;
}
//-----------------------------------------------------------------------------
uint32_t controllerGet(controller_t *controller){

    return controller->active;
}
//-----------------------------------------------------------------------------
int32_t controllerReset(controller_t *controller, uint32_t id){

    if( id >= controller->nControllers ) return CONTROLLER_ERR_INVALID_ID;

    if( controller->cbs[id].reset != 0 )
        controller->cbs[id].reset();
    
    return 0;
}
//-----------------------------------------------------------------------------
int32_t controllerSetParams(controller_t *controller, uint32_t id,
    void *params, uint32_t size){
    
    int32_t status;

    if( id >= controller->nControllers )
        return CONTROLLER_ERR_INVALID_ID;

    if( controller->cbs[id].setParams == 0 )
        return CONTROLLER_ERR_NO_SET_PARAM;

    status = controller->cbs[id].setParams(params, size);

    return status;
}
//-----------------------------------------------------------------------------
int32_t controllerGetParams(controller_t *controller, uint32_t id,
    void *buffer, uint32_t size){
    
    int32_t status;

    if( id >= controller->nControllers )
        return CONTROLLER_ERR_INVALID_ID;

    if( controller->cbs[id].getParams == 0 )
        return CONTROLLER_ERR_NO_SET_PARAM;

    status = controller->cbs[id].getParams(buffer, size);

    return status;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*----------------------------- Static functions ----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t controllerSwitch(controller_t *controller,
    void *meas, int32_t nmeas,
    void *refs, int32_t nrefs,
    void *outputs, int32_t nmaxoutputs){

    int32_t status;
    uint32_t active = controller->active;
    uint32_t previous = controller->previous;

    if( (previous != controller->nControllers) && (controller->cbs[previous].lastExit != 0) ){
        status = controller->cbs[previous].lastExit(
            meas, nmeas, 
            controller->refs.buffer, controller->refs.size,
            outputs, nmaxoutputs
            );
        
        if( status < 0 ) return status;
    }

    controller->previous = active;

    if( controller->cbs[active].firstEntry != 0 ){
        status = controller->cbs[active].firstEntry(
            meas, nmeas, 
            controller->refs.buffer, controller->refs.size,
            outputs, nmaxoutputs
            );
        
        if( status < 0 ) return status;
    }

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
