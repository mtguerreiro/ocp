/*
 * ctrace_mem.h
 *
 *  Created on: 21 de ago de 2022
 *      Author: marco
 */

#ifndef CTRACE_MEM_H_
#define CTRACE_MEM_H_

//===========================================================================
/*------------------------------- Includes --------------------------------*/
//===========================================================================
#include "stdint.h"
#include "stddef.h"
#include "ctrace_circBuf.h"
//===========================================================================

//===========================================================================
/*------------------------------ Definitions ------------------------------*/
//===========================================================================
typedef enum {
	SET_REF_VALUE = 0,
	WAIT_FOR_TRIGGER = 1,
	TRIGGER_JUST_SET = 2,
	SAVE_UNTIL_END = 3,
	WAIT_FOR_RESET = 4
} trigModeFSMState_t;

typedef struct trigModeParams_t{
	trigModeFSMState_t fsmState;
	int32_t trigBound;
	size_t numPreTrigSamples;
	size_t traceToTrack;
	float refValue;
} trigModeParams_t;

typedef struct ctracemem_t{
	size_t *start;
	size_t *end;
	size_t *p;
	circBuf_t* trigModeBuffer;
	trigModeParams_t trigModeParams;
	int32_t maxsize;
}ctracemem_t;

//===========================================================================

//===========================================================================
/*------------------------------- Functions -------------------------------*/
//===========================================================================
//---------------------------------------------------------------------------
void ctracememInitialize(ctracemem_t *trace, void *mem, int32_t size);
//---------------------------------------------------------------------------
void ctracememAddress(ctracemem_t *trace, void *address);
//---------------------------------------------------------------------------
int32_t ctracememGetSize(ctracemem_t *trace);
//---------------------------------------------------------------------------
int32_t ctracememSetSize(ctracemem_t *trace, int32_t size);
//---------------------------------------------------------------------------
void ctracememReset(ctracemem_t *trace);
//---------------------------------------------------------------------------
void ctracememSave(ctracemem_t *trace, void **src, uint32_t size);
//---------------------------------------------------------------------------
void ctracememTrigModeReset(ctracemem_t *trace);
//---------------------------------------------------------------------------
int32_t ctracememTrigModeSetNumPreTrigSamples(ctracemem_t *trace, size_t numPreTrigSamples, size_t numTraces);
//---------------------------------------------------------------------------
int32_t ctracememTrigModeSetTraceToTrack(ctracemem_t *trace, size_t traceToTrack, size_t numTraces);
//---------------------------------------------------------------------------
int32_t ctracememTrigModeSetTrigBound(ctracemem_t *trace, int32_t trigBound);
//---------------------------------------------------------------------------
void ctracememTrigModeSave(ctracemem_t *trace, void **src, uint32_t size);
//---------------------------------------------------------------------------
int32_t ctracememTrigModeGetTail(ctracemem_t *trace);
//---------------------------------------------------------------------------
//===========================================================================

#endif /* CTRACE_MEM_H_ */
