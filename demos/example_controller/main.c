//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ocp/ocpIf.h"

#include "app/app.h"

#include "demo_threads/ocpIfThread.h"
#include "demo_threads/ocpOpilThread.h"
//=============================================================================

//=============================================================================
/*---------------------------------- Main -----------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int main(void){

    pthread_t ocpIfThreadHandle, ocpOpilThreadHandle;

    ocpIfInitialize();

    appInit();

    pthread_create( &ocpIfThreadHandle, NULL, ocpIfThread, NULL );
    pthread_create( &ocpOpilThreadHandle, NULL, ocpOpilThread, NULL );

    pthread_join( ocpIfThreadHandle, NULL );

    exit(0);
}
//-----------------------------------------------------------------------------
//=============================================================================
