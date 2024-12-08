
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"

//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

typedef enum{
	CONTROLLER_ERR_INVALID_ID = -1,
    CONTROLLER_ERR_INVALID_REF_SIZE = -2,
    CONTROLLER_ERR_INVALID_REF_BUF_SIZE = -3,
    CONTROLLER_ERR_NO_SET_PARAM = -4,
}controllerErrorEnum_t;

typedef void (*controllerGetCbs_t)(void *callbacksBuffer);

typedef int32_t (*controllerInit_t)(void);
typedef int32_t (*controllerRun_t)(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs);
typedef int32_t (*controllerSetParams_t)(void *params, uint32_t size);
typedef int32_t (*controllerGetParams_t)(void *buffer, uint32_t size);
typedef void (*controllerReset_t)(void);
typedef int32_t (*controllerFirstEntry_t)(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs);
typedef int32_t (*controllerLastExit_t)(void *meas, int32_t nmeas, void *refs, int32_t nrefs, void *outputs, int32_t nmaxoutputs);

typedef struct {
    controllerInit_t init;
    controllerRun_t run;
    controllerSetParams_t setParams;
    controllerGetParams_t getParams;
    controllerReset_t reset;
    controllerFirstEntry_t firstEntry;
    controllerLastExit_t lastExit;
}controllerCallbacks_t;

typedef struct{
    uint32_t *buffer;
    uint32_t size;
}controllerRef_t;

typedef struct{

    uint32_t active;
    uint32_t previous;
    uint32_t nControllers;

    controllerCallbacks_t *cbs;

    controllerRef_t refs;
}controller_t;

typedef struct{

    void *refBuffer;
    uint32_t refSize;

    controllerGetCbs_t *getCbs;

    controllerCallbacks_t *cbsBuffer;

    uint32_t nControllers;

}controllerConfig_t;

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void controllerInit(controller_t *controller, controllerConfig_t *config);
//-----------------------------------------------------------------------------
int32_t controllerRun(controller_t *controller,
    void *meas, int32_t nmeas, 
    void *outputs, int32_t nmaxoutputs);
//-----------------------------------------------------------------------------
int32_t controllerSetRef(controller_t *controller, void *ref, uint32_t size);
//-----------------------------------------------------------------------------
int32_t controllerGetRef(controller_t *controller, void *buffer, uint32_t size);
//-----------------------------------------------------------------------------
int32_t controllerSet(controller_t *controller, uint32_t id);
//-----------------------------------------------------------------------------
uint32_t controllerGet(controller_t *controller);
//-----------------------------------------------------------------------------
int32_t controllerReset(controller_t *controller, uint32_t id);
//-----------------------------------------------------------------------------
int32_t controllerSetParams(controller_t *controller, uint32_t id,
    void *params, uint32_t size);
//-----------------------------------------------------------------------------
int32_t controllerGetParams(controller_t *controller, uint32_t id,
    void *buffer, uint32_t size);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* CONTROLLER_H_ */
