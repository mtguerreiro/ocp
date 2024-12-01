/*
 * ctrace.h
 *
 *  Created on: 21 de ago de 2022
 *      Author: marco
 */

#ifndef CTRACE_H_
#define CTRACE_H_

//===========================================================================
/*------------------------------- Includes --------------------------------*/
//===========================================================================
#include "stdint.h"

#include "ctrace_mem.h"
//===========================================================================

//===========================================================================
/*------------------------------ Definitions ------------------------------*/
//===========================================================================
/* Config struct */
typedef struct ctraceConfig_t{

	void *mem;
	uint32_t size;

	void **data;
	char *names;
}ctraceConfig_t;

/* Save Mode enum */
typedef enum {
	CTRACE_MANUAL,
	CTRACE_TRIGGER
}ctraceMode_t;

/* Trace context */
typedef struct ctrace_t{

	uint32_t n;
	void **data;
	char *names;
	char *np;
	ctraceMode_t traceMode;
	ctracemem_t mem;
}ctrace_t;

/*
 * TODO: thread-safe?
 */
//===========================================================================

//===========================================================================
/*------------------------------- Functions -------------------------------*/
//===========================================================================
//---------------------------------------------------------------------------
void ctraceInitialize(ctrace_t *trace, ctraceConfig_t *config);
//---------------------------------------------------------------------------
void ctraceAddSignal(ctrace_t *trace, void *src, char *name);
//---------------------------------------------------------------------------
void ctraceGetAddress(ctrace_t *trace, void *address);
//---------------------------------------------------------------------------
void ctraceReset(ctrace_t *trace);
//---------------------------------------------------------------------------
uint32_t ctraceGetSize(ctrace_t *trace);
//---------------------------------------------------------------------------
int32_t ctraceSetSize(ctrace_t *trace, int32_t size);
//---------------------------------------------------------------------------
int32_t ctraceGetNumberSignals(ctrace_t *trace);
//---------------------------------------------------------------------------
int32_t ctraceGetSignalsNames(ctrace_t *trace, char *buffer, int32_t maxsize);
//---------------------------------------------------------------------------
void ctraceSave(ctrace_t *trace);
//---------------------------------------------------------------------------
void ctraceTrigModeReset(ctrace_t *trace);
//---------------------------------------------------------------------------
int32_t ctraceTrigModeSetNumPreTrigSamples(ctrace_t *trace, uint32_t numPreTrigSamples);
//---------------------------------------------------------------------------
int32_t ctraceTrigModeSetTraceToTrack(ctrace_t *trace, uint32_t traceToTrack);
//---------------------------------------------------------------------------
int32_t ctraceTrigModeSetTrigBound(ctrace_t *trace, int32_t trigBound);
//---------------------------------------------------------------------------
void ctraceTrigModeSave(ctrace_t *trace);
//---------------------------------------------------------------------------
int32_t ctraceTrigModeGetTail(ctrace_t *trace);
//---------------------------------------------------------------------------
//===========================================================================

#endif /* CTRACE_H_ */
