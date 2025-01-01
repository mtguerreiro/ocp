/*
 * ctrace_mem.c
 *
 *  Created on: 21 de ago de 2022
 *      Author: marco
 */

//===========================================================================
/*------------------------------- Includes --------------------------------*/
//===========================================================================
#include "ctrace_mem.h"

//===========================================================================

//===========================================================================
/*------------------------------- Functions -------------------------------*/
//===========================================================================
//---------------------------------------------------------------------------
void ctracememInitialize(ctracemem_t *trace, void *mem, int32_t size){

	trace->start = mem;
	trace->end = mem + size;
	trace->p = mem;

	trace->maxsize = size;
}
//---------------------------------------------------------------------------
void ctracememAddress(ctracemem_t *trace, void *address){

	size_t *p = (size_t *)address;

	*p = (size_t)( trace->start );
}
//---------------------------------------------------------------------------
int32_t ctracememGetSize(ctracemem_t *trace){

	int32_t size;

	size = (trace->end - trace->start) * sizeof(ctracemem_size_t);

	return size;
}
//---------------------------------------------------------------------------
int32_t ctracememSetSize(ctracemem_t *trace, int32_t size){

	if( size > trace->maxsize ) return -1;

	size = size / sizeof(ctracemem_size_t);

	trace->end = trace->start + size;
	trace->p = trace->start;

	return 0;
}
//---------------------------------------------------------------------------
void ctracememReset(ctracemem_t *trace){

	trace->p = trace->start;
}
//---------------------------------------------------------------------------
void ctracememSave(ctracemem_t *trace, void **src, uint32_t size){

	ctracemem_size_t **s = (ctracemem_size_t **)src;

	size = size / sizeof(ctracemem_size_t);

	if( (trace->p + size) > trace->end ) return;

	while(size--) *trace->p++ = **s++;
}
//---------------------------------------------------------------------------
void ctracememTrigModeReset(ctracemem_t *trace){

	trace->trigModeBuffer.mem = trace->start;
	trace->trigModeBuffer.head = 0;
	trace->trigModeBuffer.tail = 0;
	trace->trigModeBuffer.size = trace->end - trace->start;

	trace->trigModeParams.fsmState = SET_REF_VALUE;
}
//---------------------------------------------------------------------------
int32_t ctracememTrigModeSetNumPreTrigSamples(ctracemem_t *trace, uint32_t numPreTrigSamples, uint32_t numTraces){

	if ( numPreTrigSamples > (trace->end - trace->start) / numTraces ) return -1;

	trace->trigModeParams.numPreTrigSamples = numPreTrigSamples;

	return 0;
}
//---------------------------------------------------------------------------
uint32_t ctracememTrigModeGetNumPreTrigSamples(ctracemem_t *trace){

	return trace->trigModeParams.numPreTrigSamples;
}
//---------------------------------------------------------------------------
int32_t ctracememTrigModeSetTrigSignal(ctracemem_t *trace, uint32_t signal, uint32_t numTraces){

	if (signal >= numTraces) return -1;

	trace->trigModeParams.signal = signal;

	return 0;
}
//---------------------------------------------------------------------------
uint32_t ctracememTrigModeGetTrigSignal(ctracemem_t *trace){

	return trace->trigModeParams.signal;
}
//---------------------------------------------------------------------------
void ctracememTrigModeSetTrigLevel(ctracemem_t *trace, float level){

	trace->trigModeParams.level = level;
}
//---------------------------------------------------------------------------
float ctracememTrigModeGetTrigLevel(ctracemem_t *trace){

	return trace->trigModeParams.level;
}
//---------------------------------------------------------------------------
void ctracememTrigModeSave(ctracemem_t *trace, void **src, uint32_t size){

	ctracemem_size_t **s = (ctracemem_size_t **)src;

	size = size / sizeof(ctracemem_size_t);
	uint32_t sampleCounter = size;

	switch( trace->trigModeParams.fsmState ) {
	case SET_REF_VALUE:
	{
		const uint32_t signal = trace->trigModeParams.signal;
		float floatValue = *((float*)&(**(s + signal)));
		trace->trigModeParams.refValue = floatValue;
		while(sampleCounter--) circBuf_write( &trace->trigModeBuffer, **s++ );
		trace->trigModeParams.fsmState = WAIT_FOR_TRIGGER;
	}
	break;

	case WAIT_FOR_TRIGGER:
	{
		const float level = trace->trigModeParams.level;
		const float refValue = trace->trigModeParams.refValue;
		const uint32_t signal = trace->trigModeParams.signal;
		float floatValue = *((float*)&(**(s + signal)));
		if ( (level >= refValue && floatValue >= level) || (level < refValue && floatValue <= level)  ) {
			trace->trigModeParams.fsmState = TRIGGER_JUST_SET;
		}
		while(sampleCounter--) circBuf_write( &trace->trigModeBuffer, **s++ );
	}
	break;

	case TRIGGER_JUST_SET:
	{
		const uint32_t numPreTrigSamples = trace->trigModeParams.numPreTrigSamples;
		circBuf_SetTail( &trace->trigModeBuffer, size * numPreTrigSamples );
		while(sampleCounter--) circBuf_write( &trace->trigModeBuffer, **s++ );
		trace->trigModeParams.fsmState = SAVE_UNTIL_END;
	}
	break;

	case SAVE_UNTIL_END:
	{
		while(sampleCounter--) circBuf_write( &trace->trigModeBuffer, **s++ );
		if( trace->trigModeBuffer.head == trace->trigModeBuffer.tail)
			trace->trigModeParams.fsmState = WAIT_FOR_RESET;
	}
	break;

	case WAIT_FOR_RESET:
		return;
	break;
	}
}
//---------------------------------------------------------------------------
int32_t ctracememTrigModeGetTail(ctracemem_t *trace){
	
	return trace->trigModeBuffer.tail;
}
//---------------------------------------------------------------------------
int32_t ctracememTrigModeGetState(ctracemem_t *trace){
	
	return trace->trigModeParams.fsmState;
}
//---------------------------------------------------------------------------
//===========================================================================
