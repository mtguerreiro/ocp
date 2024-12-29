/*
 * ocpIf.h
 *
 *  Created on: 17.02.2023
 *      Author: mguerreiro
 */

#ifndef OCP_IF_H_
#define OCP_IF_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#ifdef SOC_CPU0
#define OCP_IF_CONFIG_CORE	1
#else
#define OCP_IF_CONFIG_CORE	0
#endif

typedef enum{
    /**
     * Command
     *  -----------------------------------------
     *  | TRACE READ (uint32) | TRACE ID (uint32) |
     *  -----------------------------------------
     *
     * Response
     *  ------------------------
     *  | TRACE DATA (N bytes) |
     *  ------------------------
     *
     *  Notes
     *  - There is no specified format for the trace data. It is up to the user
     *    to decode these values.
     */
	OCP_IF_CMD_TRACE_READ = 0,

    /**
     * Command
     *  --------------------------------------------
     *  | RESET TRACE (uint32) | TRACE ID (uint32) |
     *  --------------------------------------------
     *
     * Response
     * No response
     */
	OCP_IF_CMD_TRACE_RESET,

    /**
     * Command
     *  -----------------------------------------------
     *  | GET TRACE SIZE (uint32) | TRACE ID (uint32) |
     *  -----------------------------------------------
     *
     * Response
     *  -----------------------
     *  | TRACE SIZE (uint32) |
     *  -----------------------
     */
	OCP_IF_CMD_TRACE_GET_SIZE,

    /**
     * Command
     *  ---------------------------------------------------------------
     *  | SET TRACE SIZE (uint32) | TRACE ID (uint32) | SIZE (uint32) |
     *  ---------------------------------------------------------------
     *
     * Response
     * No response
     *
     * Notes
     * - The size of the trace is always in bytes. Thus, if SIZE is 10,
     *   10 bytes will be allocated to the specified trace.
     * - This command will fail if the required size is larger than the
     *   maximum allowed size.
     */
	OCP_IF_CMD_TRACE_SET_SIZE,

    /**
     * Command
     *  --------------------------------------------------------
     *  | GET # OF TRACED SIGNALS (uint32) | TRACE ID (uint32) |
     *  --------------------------------------------------------
     *
     * Response
     *  --------------------------------
     *  | # OF TRACED SIGNALS (uint32) |
     *  --------------------------------
     */
	OCP_IF_CMD_TRACE_GET_NUMBER_SIGNALS,

    /**
     * Command
     *  ------------------------------------------------------------
     *  | GET NAMES OF TRACED SIGNALS (uint32) | TRACE ID (uint32) |
     *  ------------------------------------------------------------
     *
     * Response
     *  -------------------------------------
     *  | NAMES OF TRACED SIGNALS (N bytes) |
     *  -------------------------------------
     *
     * Notes
     * - Names are returned as strings, each terminated by '\0'.
     */
	OCP_IF_CMD_TRACE_GET_SIGNALS_NAMES,

    /**
     * Command
     *  ---------------------------------
     *  | GET NUMBER OF TRACES (uint32) |
     *  ---------------------------------
     *
     * Response
     *  -----------------------------
     *  | NUMBER OF TRACES (uint32) |
     *  -----------------------------
     *
     * Notes
     * - Names are returned as strings, each terminated by '\0'.
     */
	OCP_IF_CMD_TRACE_GET_NUMBER_TRACES,

    /**
     * Command
     *  --------------------------------
     *  | GET NAMES OF TRACES (uint32) |
     *  --------------------------------
     *
     * Response
     *  -----------------------------
     *  | NAMES OF TRACES (N bytes) |
     *  -----------------------------
     *
     * Notes
     * - Names are returned as strings, each terminated by '\0'.
     */
	OCP_IF_CMD_TRACE_GET_TRACES_NAMES,

    /**
     * Command
     *  ---------------------------------------------------
     *  | GET TRACE ADDRESS (uint32) |  TRACE ID (uint32) |
     *  ---------------------------------------------------
     *
     * Response
     *  --------------------------
     *  | TRACE ADDRESS (uint32) |
     *  --------------------------
     */
	OCP_IF_CMD_TRACE_GET_ADDRESS,

	/**
	 * Command
	 *  ----------------------------------------------------------
	 *  | SET MODE (uint32) |  TRACE ID (uint32) | MODE (uint32) |
	 *  ----------------------------------------------------------
	 *
	 * Response
	 * No Response
	 */
	OCP_IF_CMD_TRACE_SET_MODE,

	/**
	 * Command
	 *  ------------------------------------------
	 *  | GET MODE (uint32) |  TRACE ID (uint32) |
	 *  ------------------------------------------
	 *
	 * Response
     *  -----------------
     *  | MODE (uint32) |
     *  -----------------
	 */
	OCP_IF_CMD_TRACE_GET_MODE,

	/**
	 * Command
	 *  ---------------------------------------------------------------------------------
	 *  | SET NUM PRE TRIG SAMPLES (uint32) |  TRACE ID (uint32) | NUM SAMPLES (uint32) |
	 *  ---------------------------------------------------------------------------------
	 *
	 * Response
	 * No Response
	 */
	OCP_IF_CMD_TRACE_SET_NUM_PRE_TRIG_SAMPLES,

	/**
	 * Command
	 *  ----------------------------------------------------------
	 *  | GET NUM PRE TRIG SAMPLES (uint32) |  TRACE ID (uint32) |
	 *  ----------------------------------------------------------
	 *
	 * Response
     *  ------------------------
     *  | NUM SAMPLES (uint32) |
     *  ------------------------
	 */
	OCP_IF_CMD_TRACE_GET_NUM_PRE_TRIG_SAMPLES,

	/**
	 * Command
	 *  -------------------------------------------------------------------
	 *  | SET TRIG SIGNAL (uint32) |  TRACE ID (uint32) | SIGNAL (uint32) |
	 *  -------------------------------------------------------------------
	 *
	 * Response
	 * No Response
	 */
	OCP_IF_CMD_TRACE_SET_TRIG_SIGNAL,

	/**
	 * Command
	 *  -------------------------------------------------
	 *  | GET TRIG SIGNAL (uint32) |  TRACE ID (uint32) |
	 *  -------------------------------------------------
	 *
	 * Response
     *  --------------------
     *  | TRIGGER (uint32) |
     *  --------------------
	 */
	OCP_IF_CMD_TRACE_GET_TRIG_SIGNAL,

	/**
	 * Command
	 *  ----------------------------------------------------------------
	 *  | SET TRIG LEVEL (uint32) |  TRACE ID (uint32) | LEVEL (float) |
	 *  ----------------------------------------------------------------
	 *
	 * Response
	 * No Response
     * 
     * Notes
     *  - The size of LEVEL is 4 bytes.
	 */
	OCP_IF_CMD_TRACE_SET_TRIG_LEVEL,

	/**
	 * Command
	 *  ----------------------------------------------------------------
	 *  | SET TRIG LEVEL (uint32) |  TRACE ID (uint32) | LEVEL (float) |
	 *  ----------------------------------------------------------------
	 *
	 * Response
     *  -----------------
     *  | LEVEL (float) |
     *  -----------------
     * 
     * Notes
     *  - The size of LEVEL is 4 bytes.
	 */
	OCP_IF_CMD_TRACE_GET_TRIG_LEVEL,

	/**
	 * Command
	 *  ------------------------------------------
	 *  | GET TAIL (uint32) |  TRACE ID (uint32) |
	 *  ------------------------------------------
	 *
	 * Response
     *  -----------------
     *  | TAIL (uint32) |
     *  -----------------
	 * 
     * Notes
     *  - TAIL is the tail position of the circular buffer when the trace is
     *    in trigger mode.
	 */
	OCP_IF_CMD_TRACE_GET_TAIL,

    /**
     * Command
     *  ----------------------------------------------
     *  | GET STATUS OF CS (uint32) | CS ID (uint32) |
     *  ----------------------------------------------
     *
     * Response
     *  ----------------------
     *  | CS STATUS (uint32) |
     *  ----------------------
     *
     * Notes
     * - The meaning of the status is defined in the "ocpCSStatus_t"
     *   declaration in "ocpCS.h".
     */
	OCP_IF_CMD_CS_STATUS,

    /**
     * Command
     *  ---------------------------------------
     *  | ENABLE CS (uint32) | CS ID (uint32) |
     *  ---------------------------------------
     *
     * Response
     * No response
     */
	OCP_IF_CMD_CS_ENABLE,

    /**
     * Command
     *  ----------------------------------------
     *  | DISABLE CS (uint32) | CS ID (uint32) |
     *  ----------------------------------------
     *
     * Response
     * No response
     */
	OCP_IF_CMD_CS_DISABLE,

    /**
     * Command
     *  --------------------------------------------------------------------
     *  | ACCESS CTL. IF. OF CS (uint32) | CS ID (uint32) | DATA (N bytes) |
     *  --------------------------------------------------------------------
     *
     * Response
     *  ------------------
     *  | DATA (M bytes) |
     *  ------------------
     *
     * Note
     * - The contents of the response depends on the controller interface,
     *   and commands may or may not have responses.
     */
	OCP_IF_CMD_CS_CONTROLLER_IF,

    /**
     * Command
     *  ------------------------------------------------------------------
     *  | ACCESS HW IF. OF CS (uint32) | CS ID (uint32) | DATA (N bytes) |
     *  ------------------------------------------------------------------
     *
     * Response
     *  ------------------
     *  | DATA (M bytes) |
     *  ------------------
     *
     * Note
     * - The contents of the response depends on the hardware interface,
     *   and commands may or may not have responses.
     */
	OCP_IF_CMD_CS_HARDWARE_IF,

    /**
     * Command
     *  -------------------------
     *  | GET # OF CSs (uint32) |
     *  -------------------------
     *
     * Response
     *  ---------------------
     *  | # OF CSs (uint32) |
     *  ---------------------
     *
     */
	OCP_IF_CMD_CS_GET_NUMBER_CONTROLLERS,

    /**
     * Command
     *  -----------------------------
     *  | GET NAMES OF CSs (uint32) |
     *  -----------------------------
     *
     * Response
     *  --------------------------
     *  | NAMES OF CSs (N bytes) |
     *  --------------------------
     *
     * Notes
     * - Names are returned as strings, each terminated by '\0'.
     */
	OCP_IF_CMD_CS_GET_CONTROLLERS_NAMES,

	OCP_IF_CMD_PLATFORM_ID,
	OCP_IF_CMD_PLATFORM_IF,
	OCP_IF_CMD_OPIL_UPDATE_MEASUREMENTS,
    OCP_IF_CMD_OPIL_UPDATE_SIM_DATA,
    OCP_IF_CMD_OPIL_RUN_CONTROL,
    OCP_IF_CMD_OPIL_INITIALIZE_CONTROL,
    OCP_IF_CMD_OPIL_GET_CONTROL,
    OCP_IF_CMD_OPIL_GET_CONTROLLER_DATA,
	OCP_IF_CMD_END
}ocpIfCommands_t;

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t ocpIfInitialize(void);
//-----------------------------------------------------------------------------
int32_t ocpIf(void *in, int32_t insize, void **out, int32_t maxoutsize);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* OCP_IF_H_ */
