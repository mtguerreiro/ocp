
//===========================================================================
/*------------------------------- Includes --------------------------------*/
//===========================================================================
#include "ctrace.h"
//===========================================================================

//===========================================================================
/*------------------------------ Definitions ------------------------------*/
//===========================================================================

/*
 * TODOs
 *  - limit signal name
 *  - thread-safe?
 */
//===========================================================================

//===========================================================================
/*------------------------------- Functions -------------------------------*/
//===========================================================================
//---------------------------------------------------------------------------
void ctraceInitialize(ctrace_t *trace, ctraceConfig_t *config){

	ctracememInitialize( &trace->mem, config->mem, config->size );

	trace->data = config->data;
	trace->names = config->names;
	trace->np = config->names;

	trace->n = 0;
}
//---------------------------------------------------------------------------
void ctraceAddSignal(ctrace_t *trace, void *src, char *name){

	uint32_t n;

	n = trace->n;

	trace->data[n] = src;

	while( *name ) *(trace->np++) = *name++;
	*(trace->np++) = 0;

	trace->n++;
}
//---------------------------------------------------------------------------
void ctraceGetAddress(ctrace_t *trace, void *address){

	ctracememAddress( &trace->mem, address );
}
//---------------------------------------------------------------------------
void ctraceReset(ctrace_t *trace){

	ctracememReset( &trace->mem );
}
//---------------------------------------------------------------------------
uint32_t ctraceGetSize(ctrace_t *trace){

	return ctracememGetSize( &trace->mem );
}
//---------------------------------------------------------------------------
int32_t ctraceSetSize(ctrace_t *trace, int32_t size){

	return ctracememSetSize( &trace->mem, size );
}
//---------------------------------------------------------------------------
int32_t ctraceGetNumberSignals(ctrace_t *trace){

	return trace->n;
}
//---------------------------------------------------------------------------
int32_t ctraceGetSignalsNames(ctrace_t *trace, char *buffer, int32_t maxsize){

	int32_t k;
	char *p;

	p = trace->names;
	k = 0;
	while( (p < trace->np) && (k < maxsize) ){
		*buffer++ = *p++;
		k++;
	}

	return k;
}
//---------------------------------------------------------------------------
void ctraceSave(ctrace_t *trace){

	uint32_t size;

	size = sizeof(ctracemem_size_t) * trace->n;

	if( trace->traceMode == CTRACE_MANUAL )
		ctracememSave( &trace->mem, trace->data, size );
	else if( trace->traceMode == CTRACE_TRIGGER )
		ctracememTrigModeSave( &trace->mem, trace->data, size );
}
//---------------------------------------------------------------------------
void ctraceTrigModeReset(ctrace_t *trace){

	ctracememTrigModeReset( &trace->mem );
}
//---------------------------------------------------------------------------
int32_t ctraceTrigModeSetNumPreTrigSamples(ctrace_t *trace, uint32_t numPreTrigSamples){

	return ctracememTrigModeSetNumPreTrigSamples( &trace->mem, numPreTrigSamples, trace->n );
}
//---------------------------------------------------------------------------
uint32_t ctraceTrigModeGetNumPreTrigSamples(ctrace_t *trace){

	return ctracememTrigModeGetNumPreTrigSamples( &trace->mem );
}
//---------------------------------------------------------------------------
int32_t ctraceTrigModeSetTrigSignal(ctrace_t *trace, uint32_t signal){

	return ctracememTrigModeSetTrigSignal( &trace->mem, signal, trace->n );
}
//---------------------------------------------------------------------------
uint32_t ctraceTrigModeGetTrigSignal(ctrace_t *trace){

	return ctracememTrigModeGetTrigSignal( &trace->mem );
}
//---------------------------------------------------------------------------
void ctraceTrigModeSetTrigLevel(ctrace_t *trace, float level){

	ctracememTrigModeSetTrigLevel( &trace->mem, level );
}
//---------------------------------------------------------------------------
float ctraceTrigModeGetTrigLevel(ctrace_t *trace){

	return ctracememTrigModeGetTrigLevel( &trace->mem );
}
//---------------------------------------------------------------------------
int32_t ctraceTrigModeGetTail(ctrace_t *trace){

	return ctracememTrigModeGetTail(&trace->mem);
}
//---------------------------------------------------------------------------
int32_t ctraceTrigModeGetState(ctrace_t *trace){

	return ctracememTrigModeGetState( &trace->mem );
}
//---------------------------------------------------------------------------
//===========================================================================
