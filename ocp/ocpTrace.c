/*
 * ocpTrace.c
 *
 *  Created on: 17.02.2023
 *      Author: mguerreiro
 */

//===========================================================================
/*------------------------------- Includes --------------------------------*/
//===========================================================================
#include "ocpTrace.h"

#include "ocpConfig.h"
//===========================================================================

//===========================================================================
/*------------------------------ Definitions ------------------------------*/
//===========================================================================

typedef ctrace_t ocpTrace_t;

typedef struct ocpTraceControl_t{

	ocpTrace_t traces[OCP_TRACE_END];

	char names[OCP_TRACE_END * OCP_TRACE_CONFIG_TRACE_NAME_MAX_LEN];

	char *np;
}ocpTraceControl_t;
//===========================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================

static ocpTraceControl_t xifcontrol = {.np = xifcontrol.names};

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t ocpTraceInitialize(uint32_t id, ocpTraceConfig_t *config, char *name){

	char *tplim;

	if( id >= OCP_TRACE_END ) return -1;

	ctraceInitialize( &xifcontrol.traces[id] , config );

	tplim = xifcontrol.names + OCP_TRACE_END * OCP_TRACE_CONFIG_TRACE_NAME_MAX_LEN;
	while( *name && (xifcontrol.np < tplim) ) *xifcontrol.np++ = *name++;
	*xifcontrol.np++ = 0;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceAddSignal(uint32_t id, void *src, char *name){

	if( id >= OCP_TRACE_END ) return -1;

	ctraceAddSignal( &xifcontrol.traces[id] , src, name );

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetAddress(uint32_t id, void *address){

	if( id >= OCP_TRACE_END ) return -1;

	ctraceGetAddress( &xifcontrol.traces[id], address );

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceReset(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	switch ( xifcontrol.traces[id].traceMode ) {
	case CTRACE_MANUAL:
	default:
		ctraceReset( &xifcontrol.traces[id] );
	break;
	case CTRACE_TRIGGER:
		ctraceTrigModeReset( &xifcontrol.traces[id] );
	break;
	}

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetSize(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceGetSize( &xifcontrol.traces[id] );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceSetSize(uint32_t id, int32_t size){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceSetSize( &xifcontrol.traces[id], size );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetNumberSignals(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceGetNumberSignals( &xifcontrol.traces[id] );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetSignalsNames(uint32_t id, char *buffer, int32_t maxsize){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceGetSignalsNames( &xifcontrol.traces[id], buffer, maxsize );
}
//---------------------------------------------------------------------------
int32_t ocpTraceGetNumberTraces(void){

	return OCP_TRACE_END;
}
//---------------------------------------------------------------------------
int32_t ocpTraceGetTracesNames(char *buffer, int32_t maxsize){

	int32_t k;
	char *p;

	p = xifcontrol.names;
	k = 0;
	while( (p < xifcontrol.np) && (k < maxsize) ){
		*buffer++ = *p++;
		k++;
	}

	return k;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceSetMode(uint32_t id, ocpTraceMode_t mode){

	if( id >= OCP_TRACE_END ) return -1;

	if( (mode != CTRACE_TRIGGER) && (mode != CTRACE_MANUAL) )
		return -2;
	
	xifcontrol.traces[id].traceMode = mode;
	ocpTraceReset(id);

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetMode(uint32_t id){

	int32_t mode;

	if( id >= OCP_TRACE_END ) return -1;

	mode = (int32_t) xifcontrol.traces[id].traceMode;

	return mode;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceSetNumPreTrigSamples(uint32_t id, uint32_t samples){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceTrigModeSetNumPreTrigSamples( &xifcontrol.traces[id], samples );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetNumPreTrigSamples(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	return (int32_t) ctraceTrigModeGetNumPreTrigSamples( &xifcontrol.traces[id] );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceSetTrigSignal(uint32_t id, uint32_t signal){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceTrigModeSetTrigSignal( &xifcontrol.traces[id], signal );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetTrigSignal(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	return (int32_t) ctraceTrigModeGetTrigSignal( &xifcontrol.traces[id] );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceSetTrigLevel(uint32_t id, float level){

	if( id >= OCP_TRACE_END ) return -1;

	ctraceTrigModeSetTrigLevel( &xifcontrol.traces[id], level );

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetTrigLevel(uint32_t id, float *level){

	if( id >= OCP_TRACE_END ) return -1;

	*level = ctraceTrigModeGetTrigLevel( &xifcontrol.traces[id] );

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ocpTraceGetTail(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	return ctraceTrigModeGetTail( &xifcontrol.traces[id] );
}
//-----------------------------------------------------------------------------
int32_t ocpTraceSave(uint32_t id){

	if( id >= OCP_TRACE_END ) return -1;

	ctraceSave( &xifcontrol.traces[id] );

	return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
