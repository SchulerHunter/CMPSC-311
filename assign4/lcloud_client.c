////////////////////////////////////////////////////////////////////////////////
//
//  File          : lcloud_client.c
//  Description   : This is the client side of the Lion Clound network
//                  communication protocol.
//
//  Author        : Hunter Schuler
//  Last Modified : 4/11/2020
//

// Include Files
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>

// Project Include Files
#include <lcloud_filesys.h>
#include <lcloud_network.h>
#include <cmpsc311_log.h>

// Local functional prototypes
int lcloud_client_connect( unsigned char *ip, uint16_t port );

//
//  Global data
int client_socket = -1;
int                lcloud_network_shutdown = 0;   // Flag indicating shutdown
unsigned char     *lcloud_network_address = NULL; // Address of LCloud server
unsigned short     lcloud_network_port = 0;       // Port of LCloud server

//
// Functions
LCloudRegisterFrame hton64(LCloudRegisterFrame register) {

}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : client_lcloud_bus_request
// Description  : This the client regstateeration that sends a request to the 
//                lion client server.   It will:
//
//                1) if INIT make a connection to the server
//                2) send any request to the server, returning results
//                3) if CLOSE, will close the connection
//
// Inputs       : reg - the request reqisters for the command
//                buf - the block to be read/written from (READ/WRITE)
// Outputs      : the response structure encoded as needed
LCloudRegisterFrame client_lcloud_bus_request( LCloudRegisterFrame reg, void *buf ) {
    if (client_socket == -1) {
        // Initialize the connection
    }
}

