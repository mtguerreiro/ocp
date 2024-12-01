#ifndef CTRACE_CIRCBUF_H_
#define CTRACE_CIRCBUF_H_

//===========================================================================
/*------------------------------- Includes --------------------------------*/
//===========================================================================
#include "stdint.h"
#include "stddef.h"

//===========================================================================

//===========================================================================
/*------------------------------ Definitions ------------------------------*/
//===========================================================================
typedef uint32_t ctrace_circ_buf_size_t;

typedef struct {
	ctrace_circ_buf_size_t* mem;
	int32_t head;
	int32_t tail;
	uint32_t size;
}circBuf_t;
//===========================================================================

//===========================================================================
/*------------------------------- Functions -------------------------------*/
//===========================================================================
//---------------------------------------------------------------------------
/**
 * @brief Writes to circular buffer.
 *
 * @param buffer Pointer to circular buffer.
 * @param data Value to be written.
 */
void circBuf_write( circBuf_t* buffer, int32_t data );
//---------------------------------------------------------------------------
/**
 * @brief Sets tail position relative to head position of circular buffer.
 *
 * @param buffer Pointer to circular buffer.
 * @param distance Amount of positions to move backwards from the head position until setting the tail.
 */
void circBuf_SetTail( circBuf_t* buffer, int32_t distance );
//---------------------------------------------------------------------------
//===========================================================================

#endif /* CTRACE_CIRCBUF_H_ */
