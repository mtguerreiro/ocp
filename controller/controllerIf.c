
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "controllerIf.h"

#include "../rp/rp.h"

#include "ocpConfig.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static controller_t *controllers[OCP_CS_END] = {0};
static rpctx_t rp;
static rphandle_t handles[CONTROLLER_IF_END];
//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
static int32_t controllerIfSetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t controllerIfGetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t controllerIfSet(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t controllerIfGet(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t controllerIfReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t controllerIfSetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
static int32_t controllerIfGetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void controllerIfInit(void){

    rpInitialize(&rp, CONTROLLER_IF_END, handles);
    
    rpRegisterHandle(&rp, CONTROLLER_IF_SET_REF, controllerIfSetReferences);
    rpRegisterHandle(&rp, CONTROLLER_IF_GET_REF, controllerIfGetReferences);
    rpRegisterHandle(&rp, CONTROLLER_IF_SET, controllerIfSet);
    rpRegisterHandle(&rp, CONTROLLER_IF_GET, controllerIfGet);
    rpRegisterHandle(&rp, CONTROLLER_IF_RESET, controllerIfReset);
    rpRegisterHandle(&rp, CONTROLLER_IF_SET_PARAMS, controllerIfSetParams);
    rpRegisterHandle(&rp, CONTROLLER_IF_GET_PARAMS, controllerIfGetParams);
}
//-----------------------------------------------------------------------------
int32_t controllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    return rpRequest(&rp, in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
int32_t controllerIfRegister(controller_t *controller, uint32_t csid){

    if( csid >= OCP_CS_END ) return -1;

    controllers[csid] = controller;

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*----------------------------- Static functions ----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static int32_t controllerIfSetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p = (uint32_t *)in;
    uint32_t csid;

    csid = *p++;

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerSetRef(controllers[csid], (void *)p, insize - 4);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfGetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p = (uint32_t *)in;
    uint32_t csid;
    void *o = (void *)*out;

    csid = *p++;

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerGetRef(controllers[csid], o, maxoutsize);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfSet(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p = (uint32_t *)in;
    uint32_t csid;    
    uint32_t ctl;

    csid = *p++;
    ctl = *p++;

   if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerSet(controllers[csid], ctl);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfGet(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t ctl;
    uint32_t *p = (uint32_t  *)*out;
    uint32_t csid = *((uint32_t *)in);

    if( csid >= OCP_CS_END ) return -1;
    
    if( controllers[csid] != 0 )
        ctl = controllerGet(controllers[csid]);

    *p = ctl;

    return 4;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p = (uint32_t *)in;
    uint32_t csid;    
    uint32_t ctl;

    csid = *p++;
    ctl = *p++;

    if( controllers[csid] != 0 )
        status = controllerReset(controllers[csid], ctl);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfSetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p = (uint32_t *)in;
    uint32_t csid;    
    uint32_t ctl;

    csid = *p++;
    ctl = *p++;

    if( controllers[csid] != 0 )
        status = controllerSetParams(controllers[csid], ctl, (void *)p, insize - 8);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfGetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status;
    uint32_t *p = (uint32_t *)in;
    uint32_t csid;    
    uint32_t ctl;

    csid = *p++;
    ctl = *p++;

    if( controllers[csid] != 0 )
        status = controllerGetParams(controllers[csid], ctl, *out, maxoutsize);

    return status;
}
//-----------------------------------------------------------------------------
//=============================================================================
