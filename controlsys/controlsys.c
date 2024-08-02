/*
 * @file controlsys.h
 *
 * @brief A generic control system written in C.
 */

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "controlsys.h"
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void controlsysInitialize(controlsys_t *sys, controlsysConfig_t *config){

	sys->binputs = config->binputs;
	sys->boutputs = config->boutputs;

	sys->fgetInputs = config->fgetInputs;
	sys->fapplyOutputs = config->fapplyOutputs;

	sys->frun = config->frun;

	sys->fonEntry = config->fonEntry;
	sys->fonExit = config->fonExit;

	sys->fhwInterface = config->fhwInterface;
	sys->fhwStatus = config->fhwStatus;

	sys->fcontrollerInterface = config->fcontrollerInterface;
	sys->fcontrollerStatus = config->fcontrollerStatus;

	sys->fenable = config->fenable;
	sys->fdisable = config->fdisable;

	sys->status = CONTROLSYS_STATUS_DISABLED;
}
//-----------------------------------------------------------------------------
int32_t controlsysControllerInterface(controlsys_t *sys,
		void *in, uint32_t insize,
		void **out, uint32_t maxoutsize){

	if( sys->fcontrollerInterface == 0 ) return CONTROLSYS_ERR_UNDEF;

	return sys->fcontrollerInterface(in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
int32_t controlsysHardwareInterface(controlsys_t *sys,
		void *in, uint32_t insize,
		void **out, uint32_t maxoutsize){

	if( sys->fhwInterface == 0 ) return CONTROLSYS_ERR_UNDEF;

	return sys->fhwInterface(in, insize, out, maxoutsize);
}
//-----------------------------------------------------------------------------
int32_t controlsysRun(controlsys_t *sys){

    int32_t ninputs;
    int32_t noutputs;
    int32_t status;

    if( sys->status == CONTROLSYS_STATUS_DISABLED ) return CONTROLSYS_STATUS_DISABLED;

    if( sys->fonEntry ) sys->fonEntry();

    ninputs = sys->fgetInputs( sys->binputs );
    if( ninputs < 0 ) sys->status = CONTROLSYS_STATUS_HARDWARE_ERROR;

    if( sys->status == CONTROLSYS_STATUS_ENABLED ){
        noutputs = sys->frun( sys->binputs, ninputs, sys->boutputs, -1 );
        if( noutputs < 0 ) sys->status = CONTROLSYS_STATUS_CONTROLLER_ERROR;
    }

    if( sys->status == CONTROLSYS_STATUS_ENABLED ){
        status = sys->fapplyOutputs( sys->boutputs, noutputs );
        if( status != 0 ) sys->status = CONTROLSYS_STATUS_HARDWARE_ERROR;
    }

    if( sys->fonExit ) sys->fonExit();

	return sys->status;
}
//-----------------------------------------------------------------------------
int32_t controlsysEnable(controlsys_t *sys){

	if( (sys->fhwStatus() != 0) || (sys->fcontrollerStatus() != 0) ) return CONTROLSYS_ERR_EN;

	sys->status = CONTROLSYS_STATUS_ENABLED;

	if( sys->fenable != 0 ) sys->fenable();

	return 0;
}
//-----------------------------------------------------------------------------
void controlsysDisable(controlsys_t *sys){

	sys->status = CONTROLSYS_STATUS_DISABLED;

	if( sys->fdisable != 0 ) sys->fdisable();
}
//-----------------------------------------------------------------------------
int32_t controlsysStatus(controlsys_t *sys){

    return sys->status;
}
//-----------------------------------------------------------------------------
//=============================================================================
