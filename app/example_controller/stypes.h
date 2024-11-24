/*
 * stypesBuck.h
 *
 *  Created on: 24 de mai de 2023
 *      Author: LRS
 */

#ifndef S_TYPES_BUCK_H_
#define S_TYPES_BUCK_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
#include "stddef.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{
	float i;
	float v_out;
}stypesMeasurements_t;

typedef struct{
	float V_ref;
}stypesSimData_t;

typedef struct{
	float D;
}stypesControl_t;

typedef struct{
	float t_exec;
}stypesControllerData_t;
//=============================================================================

#endif /* S_TYPES_BUCK_H_ */
