
#ifndef CONTROLLER_IF_H_
#define CONTROLLER_IF_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"

#include "controller.h"
//============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef enum{
	CONTROLLER_IF_SET_REF,      /* Sets the references */
	CONTROLLER_IF_GET_REF,      /* Gets the references */
	CONTROLLER_IF_SET,          /* Sets the active controller */
	CONTROLLER_IF_GET,			/* Gets the active controller */
	CONTROLLER_IF_RESET,        /* Resets the specified controller */
	CONTROLLER_IF_SET_PARAMS,	/* Sets parameters for the specified controller */
	CONTROLLER_IF_GET_PARAMS,	/* Gets parameters for the specified controller */
	CONTROLLER_IF_END
}controllerIfEnum_t;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void controllerIfInit(void);
//-----------------------------------------------------------------------------
int32_t controllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize);
//-----------------------------------------------------------------------------
int32_t controllerIfRegister(controller_t *controller, uint32_t csid);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* CONTROLLER_IF_H_ */
