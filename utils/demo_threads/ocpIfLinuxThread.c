
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "ocpIfThread.h"

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Open controller project */
#include "ocp/ocpIf.h"
#include "ocpConfig.h"
//============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void *ocpIfThreadProcess(void *param);
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#define OCP_IF_THREAD_BACKLOG       5
#define OCP_IF_THREAD_BUFFER_SIZE   1024
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static int server_socket, client_socket;
static socklen_t client_len;
static struct sockaddr_in server_addr, client_addr;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void *ocpIfThread(void *ptr){

    (void)ptr;
    pthread_t ocpIfThreadProcessHandle;
    int status;

    /* Creates the server socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if( server_socket < 0 ){
        LogError( ("Socket failed with error code %d", server_socket) );
        return 0;
    }

    /* Sets up the server address structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(OCP_IF_THREAD_SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /* Binds the server socket to the address */
    status = bind( server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr) );
    if( status < 0 ){
        LogError( ("Socket bind failed with error code %d", status) );
        return 0;
    }

    /* Listens for incoming connections */
    status = listen(server_socket, OCP_IF_THREAD_BACKLOG); 
    if( status < 0 ){
        LogError( ("Socket listen failed with error code %d", status) );
        close(server_socket);
        return 0;
    }

    LogInfo( ("OCP interface ready and running") );

    while(1){
        /* Waits for a connection */
        client_len = sizeof(client_addr);
        client_socket = accept( server_socket , (struct sockaddr *)&client_addr, &client_len );
        if( client_socket < 0 ) {
            LogError( ("Socket accept failed with error code %d", client_socket) );
            close(server_socket);
            return 0;
        }

        LogInfo( ("Client connected from %s", inet_ntoa(client_addr.sin_addr)) );

        pthread_create( &ocpIfThreadProcessHandle, NULL, ocpIfThreadProcess, (void *)&client_socket);
        pthread_join( ocpIfThreadProcessHandle, NULL);

    }

    close(server_socket);

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void *ocpIfThreadProcess(void *param){

    int sd = *( (int *)param );
    char recvbuf[OCP_IF_THREAD_BUFFER_SIZE];
    int32_t n;
    int32_t nrx;
    int32_t ret;
    int32_t size;

    int32_t status;

    uint8_t *p;

    while(1){
        /*
         * After receiving a connection, reads the first 4 bytes. They should
         * represent the number of bytes to come next.
         */
        nrx = 0;
        while( nrx < 4 ){
            n = recv(sd, (void *)&recvbuf[nrx], 4 - nrx, 0);
            if( n <= 0 ){
                LogError( ("Socket recv failed with error code %d", n) );
                break;
            }
            nrx += n;
        }
        if( nrx != 4 ){
            LogError( ("Error receiving data, expected 4 bytes but got %d", nrx) );
            break;
        }
        memcpy((void *)&size, (void *)recvbuf, nrx);

        /* Read the number of expected bytes */
        nrx = 0;
        while( nrx < size ){
            n = recv(sd, (void *)&recvbuf[nrx], size - nrx, 0);
            if( n <= 0 ){
                LogError( ("Socket recv failed with error code %d", n) );
                break;
            }
            nrx += n;
        }
        if( nrx != size ){
            LogError( ("Error receiving data, expected %d bytes but got %d", size, nrx) );
            break;
        }

        /* Calls the interface */
        p = (uint8_t *)( recvbuf );
        ret = ocpIf((void *)p, size, (void **)(&p), OCP_IF_THREAD_BUFFER_SIZE);

        /*
         * Now, sends the reply. The reply consists of the command status/size
         * (4 bytes), followed by data (if any).
         */
        // TODO: should we also do a while loop to send the data? Like in receiving?

        /* Writes back the command status */
        //*( (int32_t *)recvbuf ) = ret;
        n = send(sd, (void *)&ret, 4, 0);
        if( n < 4 ){
            LogError( ("Socket send failed with error code %d", n) );
            break;
        }

        /* Writes data */
        if( ret > 0 ){
            n = send(sd, (void *)p, ret, 0);
            if( n < ret ){
                LogError( ("Socket send failed with error code %d", n) );
            }
        }
        break;
    }

    /* Closes connection */
    status = shutdown(sd, SHUT_RDWR);
    if( status < 0 ){
        LogError( ("Socket shutdown failed with error code %d", status) );
    }

    close(sd);

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================
