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
typedef uint32_t ctracemem_size_t;

typedef enum {
	SET_REF_VALUE = 0,
	WAIT_FOR_TRIGGER = 1,
	TRIGGER_JUST_SET = 2,
	SAVE_UNTIL_END = 3,
	WAIT_FOR_RESET = 4
} trigModeFSMState_t;

typedef struct trigModeParams_t{
	trigModeFSMState_t fsmState;
	float trigBound;
	uint32_t numPreTrigSamples;
	uint32_t traceToTrack;
	float refValue;
} trigModeParams_t;

typedef struct ctracemem_t{
	ctracemem_size_t *start;
	ctracemem_size_t *end;
	ctracemem_size_t *p;
	circBuf_t* trigModeBuffer;
	trigModeParams_t trigModeParams;
	int32_t maxsize;
}ctracemem_t;
//===========================================================================

//===========================================================================
/*------------------------------- Functions -------------------------------*/
//===========================================================================
//---------------------------------------------------------------------------
/**
 * @brief Initializes the trace memory region.
 *
 * Sets start, end and maximum size of trace memory.
 *
 * @param trace Pointer to trace memory region.
 * @param mem Address which the trace pointer is assigned to, defines start of trace memory region.
 * @param size Total size of memory reserved for the traces.
 */
void ctracememInitialize(ctracemem_t *trace, void *mem, int32_t size);
//---------------------------------------------------------------------------
/**
 * @brief Writes start address of trace memory region to a given variable.
 *
 * @param trace Pointer to trace memory region.
 * @param address Reference to the variable to write the start address to.
 */
void ctracememAddress(ctracemem_t *trace, void *address);
//---------------------------------------------------------------------------
/**
 * @brief Gets the total number of bytes configured for all traces.
 *
 * This is defined by the amount of samples that are stored.
 *
 * @param trace Pointer to trace memory region.
 * @return total number of bytes configured for all traces.
 */
int32_t ctracememGetSize(ctracemem_t *trace);
//---------------------------------------------------------------------------
/**
 * @brief Sets total size of all traces.
 *
 * This defines the amount of samples that are stored.
 *
 * @param trace Pointer to trace memory region.
 * @param size Total size of all traces, determined by the product of the amount of traces and the amount of samples to be stored.
 * @return -1 if size parameter exceeds the maximum trace size, 0 otherwise.
 */
int32_t ctracememSetSize(ctracemem_t *trace, int32_t size);
//---------------------------------------------------------------------------
/**
 * @brief Resets Trace for Manual Mode.
 *
 * Sets write pointer to the start of the trace memory region.
 *
 * @param trace Pointer to trace memory region.
 */
void ctracememReset(ctracemem_t *trace);
//---------------------------------------------------------------------------
/**
 * @brief Saves measurement values for each trace in Manual Mode.
 *
 * All measurement values at the time this function is called are stored in their respective trace.
 *
 * @param trace Pointer to trace memory region.
 * @param src Contains measurement values for each trace.
 * @param size Total amount of traces (corresponds to amount of measurements).
 */
void ctracememSave(ctracemem_t *trace, void **src, uint32_t size);
//---------------------------------------------------------------------------
/**
 * @brief Resets Trace for Trig Mode.
 *
 * Initializes circular buffer and Trig Mode parameters.
 * Must be called to store new values after the trigger has been set or when the trace to track has been changed.
 * @param trace Pointer to trace memory region.
 */
void ctracememTrigModeReset(ctracemem_t *trace);
//---------------------------------------------------------------------------
/**
 * @brief Sets number of samples to be stored prior to the trigger sample.
 *
 * This determines at which position the trigger sample is seen in the plot.
 *
 * @param trace Pointer to trace memory region.
 * @param numPreTrigSamples Number of samples to be stored prior to the trigger sample.
 * @param numTraces Total number of traces, used for error generation if numPreTrigSamples exceeds the trace size.
 * @return -1 if numPreTrigSamples exceeds trace size, 0 otherwise.
 */
int32_t ctracememTrigModeSetNumPreTrigSamples(ctracemem_t *trace, size_t numPreTrigSamples, size_t numTraces);
//---------------------------------------------------------------------------
/**
 * @brief Sets particular trace for which the trigger is defined.
 *
 * The number to be set corresponds to the ID for the particular measurement.
 *
 * @param trace Pointer to trace memory region.
 * @param traceToTrack Trace for which the trigger is defined.
 * @param numTraces Total number of traces, used for error generation if traceToTrack does not belong to any measurement.
 * @return -1 if traceToTrack does not belong to a measurement, 0 otherwise.
 */
int32_t ctracememTrigModeSetTraceToTrack(ctracemem_t *trace, size_t traceToTrack, size_t numTraces);
//---------------------------------------------------------------------------
/**
 * @brief Sets the trigger bound value.
 *
 * After a trace reset, the trigger is set each time this value is crossed from above or from below.
 *
 * @param trace Pointer to trace memory region.
 * @param trigBound bound value; reinterpret cast is used to convert back to the float value set in the Python command.
 * @return Always return 0.
 */
int32_t ctracememTrigModeSetTrigBound(ctracemem_t *trace, int32_t trigBound);
//---------------------------------------------------------------------------
/**
 * @brief Saves measurement values for each trace in Trig Mode.
 *
 * The values are continuously stored in a circular buffer until the trigger is set. Implemented as a Finite State Machine.
 *
 * @param trace Pointer to trace memory region.
 * @param src Contains measurement values for each trace.
 * @param size Total amount of traces (corresponds to amount of measurements).
 */
void ctracememTrigModeSave(ctracemem_t *trace, void **src, uint32_t size);
//---------------------------------------------------------------------------
/**
 * @brief Gets tail position of circular buffer.
 *
 * This function is necessary to reorder the trace using the trig_mode_reorder command in Python.
 *
 * @param trace Pointer to trace memory region.
 * @return Tail position of circular buffer.
 */
int32_t ctracememTrigModeGetTail(ctracemem_t *trace);
//---------------------------------------------------------------------------
//===========================================================================

#endif /* CTRACE_MEM_H_ */
