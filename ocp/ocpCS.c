
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "ocpCS.h"

#include "ocpConfig.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef controlsys_t ocpCS_t;

#if OCP_CS_CONFIG_ENABLE != 0
typedef struct ocpCSControl_t{
    ocpCS_t cs[OCP_CS_END];
    char names[OCP_CS_END * OCP_CS_CONFIG_CS_NAME_MAX_LEN];
    char *np;
}ocpCSControl_t;
#endif
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
#if OCP_CS_CONFIG_ENABLE != 0
static ocpCSControl_t xcscontrol = {.np = xcscontrol.names};
#endif
//=============================================================================


//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t ocpCSInitialize(uint32_t id, ocpCSConfig_t *config, char *name){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    (void)config;
    (void)name;
    return -1;
#else
    char *tplim;

    if( id >= OCP_CS_END ) return -1;

    controlsysInitialize( &xcscontrol.cs[id], config );

    tplim = xcscontrol.names + OCP_CS_END * OCP_CS_CONFIG_CS_NAME_MAX_LEN;
    while( *name && (xcscontrol.np < (tplim - 1U)) ) *xcscontrol.np++ = *name++;
    *xcscontrol.np++ = 0;

    return 0;
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSControllerInterface(
    uint32_t id,
    void *in, uint32_t insize,
    void **out, uint32_t maxoutsize){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    (void)in;
    (void)insize;
    (void)out;
    (void)maxoutsize;
    return -1;
#else
    if( id >= OCP_CS_END ) return -1;

    return controlsysControllerInterface( &xcscontrol.cs[id], in, insize, out, maxoutsize );
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSHardwareInterface(
    uint32_t id,
    void *in, uint32_t insize,
    void **out, uint32_t maxoutsize){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    (void)in;
    (void)insize;
    (void)out;
    (void)maxoutsize;
    return -1;
#else
    if( id >= OCP_CS_END ) return -1;

    return controlsysHardwareInterface( &xcscontrol.cs[id], in, insize, out, maxoutsize);
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSRun(uint32_t id){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    return -1;
#else
    if( id >= OCP_CS_END ) return -1;

    return controlsysRun( &xcscontrol.cs[id] );
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSEnable(uint32_t id){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    return -1;
#else
    if( id >= OCP_CS_END ) return -1;

    controlsysEnable( &xcscontrol.cs[id] );

    return 0;
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSDisable(uint32_t id){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    return -1;
#else
    if( id >= OCP_CS_END ) return -1;

    controlsysDisable( &xcscontrol.cs[id] );

    return 0;
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSStatus(uint32_t id, int32_t *status){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)id;
    (void)status;
    return -1;
#else
    if( id >= OCP_CS_END ) return -1;

    *status = controlsysStatus(&xcscontrol.cs[id]);

    return 0;
#endif
}
//-----------------------------------------------------------------------------
int32_t ocpCSGetNumberControllers(void){

    return OCP_CS_END;
}
//-----------------------------------------------------------------------------
int32_t ocpCSGetControllersNames(char *buffer, int32_t maxsize){

#if OCP_CS_CONFIG_ENABLE == 0
    (void)buffer;
    (void)maxsize;
    return 0;
#else
    int32_t k;
    char *p;

    p = xcscontrol.names;
    k = 0;
    while( (p < xcscontrol.np) && (k < maxsize) ){
        *buffer++ = *p++;
        k++;
    }

    return k;
#endif
}
//-----------------------------------------------------------------------------
//=============================================================================
