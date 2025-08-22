
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "controllerIf.h"

#include "rp/rp.h"

#include "ocpConfig.h"

#include "string.h"
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

    (void)out;
    (void)maxoutsize;
    int32_t status = -1;
    char *p;
    uint32_t csid;

    if( insize < sizeof(csid) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    p = (char *)in + sizeof(csid);

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerSetRef(controllers[csid], (void *)p, insize - sizeof(csid));

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfGetReferences(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status = -1;
    uint32_t csid;

    if( insize != sizeof(csid) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerGetRef(controllers[csid], *out, maxoutsize);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfSet(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    int32_t status = -1;
    char *p;
    uint32_t csid;
    uint32_t ctl;

    if( insize < (sizeof(csid) + sizeof(ctl)) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    p = (char *)in + sizeof(csid);

    memcpy( (void *)&ctl, (void *)p, sizeof(ctl) );

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerSet(controllers[csid], ctl);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfGet(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    uint32_t ctl;
    uint32_t csid;

    if( insize < sizeof(csid) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] == 0 ) return -1;

    ctl = controllerGet(controllers[csid]);

    if( maxoutsize < sizeof(ctl) ) return -1;

    memcpy( *out, (void *)&ctl, sizeof(ctl) );

    return sizeof(ctl);
}
//-----------------------------------------------------------------------------
static int32_t controllerIfReset(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    int32_t status = -1;
    uint32_t csid;
    uint32_t ctl;
    char *p;

    if( insize < (sizeof(csid) + sizeof(ctl)) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    p = (char *)in + sizeof(csid);

    memcpy( (void *)&ctl, (void *)p, sizeof(ctl) );

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerReset(controllers[csid], ctl);

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfSetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;
    int32_t status = -1;
    char *p;
    uint32_t csid;    
    uint32_t ctl;

    if( insize < (sizeof(csid) + sizeof(ctl)) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    p = (char *)in + sizeof(csid);

    memcpy( (void *)&ctl, (void *)p, sizeof(ctl) );

    p += sizeof(ctl);

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerSetParams(
            controllers[csid], ctl,
            (void *)p, insize - ( sizeof(csid) + sizeof(ctl) )
        );

    return status;
}
//-----------------------------------------------------------------------------
static int32_t controllerIfGetParams(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    int32_t status = -1;
    char *p;
    uint32_t csid;
    uint32_t ctl;

    if( insize < (sizeof(csid) + sizeof(ctl)) ) return -1;

    memcpy( (void *)&csid, in, sizeof(csid) );

    p = (char *)in + sizeof(csid);

    memcpy( (void *)&ctl, (void *)p, sizeof(ctl) );

    if( csid >= OCP_CS_END ) return -1;

    if( controllers[csid] != 0 )
        status = controllerGetParams(controllers[csid], ctl, *out, maxoutsize);

    return status;
}
//-----------------------------------------------------------------------------
//=============================================================================
