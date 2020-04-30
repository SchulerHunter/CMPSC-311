////////////////////////////////////////////////////////////////////////////////
//
//  File          : lcloud_client.c
//  Description   : This is the client side of the Lion Clound network
//                  communication protocol.
//
//  Author        : Hunter Schuler
//  Last Modified : 4/30/2020
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
#include <lcloud_network.h>
#include <lcloud_controller.h>
#include <cmpsc311_log.h>
#include <cmpsc311_util.h>

//
//  Global data
typedef enum{READ, WRITE} xfer;
int client_socket = -1;

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcloud_client_connect
// Description  : Connects to the lcloud server
// Inputs       : ip - the ip address to connect to
//                port - the port the server is running on
// Outputs      : Socket identifier
int lcloud_client_connect(char *ip, uint16_t port){
    struct sockaddr_in server;
    if (inet_pton(AF_INET, ip, &server.sin_addr) == -1) {
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        return -1;
    }

    if(connect(client_socket, (struct sockaddr *)&server, sizeof(server)) == -1) {
        return -1;
    }

    return client_socket;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : client_lcloud_bus_request
// Description  : This the client regstateeration that sends a request to the 
//                lion client server.
// Inputs       : reg - the request reqisters for the command
//                buf - the block to be read/written from (READ/WRITE)
// Outputs      : the response structure encoded as needed
LCloudRegisterFrame client_lcloud_bus_request( LCloudRegisterFrame reg, void *buf ) {
    // Check for open connection
    if (client_socket == -1) {
        client_socket = lcloud_client_connect(LCLOUD_DEFAULT_IP, LCLOUD_DEFAULT_PORT);
        if (client_socket == -1) {
            return -1;
        }
    }

    // Extract the C0 and C2 registers from the register
    xfer C2 = (reg >> 32) & 0xFF;
    LcOperationCode C0 = (reg >> 48) & 0xFF;

    // Switch to network mode and send register
    reg = htonll64(reg);
    if (write(client_socket, &reg, LCLOUD_NET_HEADER_SIZE) != LCLOUD_NET_HEADER_SIZE) {
        return -1;
    }

    // Case of transfer
    if (C0 == LC_BLOCK_XFER) {
        if (C2 == LC_XFER_READ) {
            if (read(client_socket, &reg, LCLOUD_NET_HEADER_SIZE) != LCLOUD_NET_HEADER_SIZE) {
                return -1;
            }

            if (read(client_socket, buf, LC_DEVICE_BLOCK_SIZE) != LC_DEVICE_BLOCK_SIZE) {
                return -1;
            }
        } else if (C2 == LC_XFER_WRITE) {
            if (write(client_socket, buf, LC_DEVICE_BLOCK_SIZE) != LC_DEVICE_BLOCK_SIZE) {
                return -1;
            }

            if (read(client_socket, &reg, LCLOUD_NET_HEADER_SIZE) != LCLOUD_NET_HEADER_SIZE) {
                return -1;
            }
        } else {
            return -1;
        }
    // All other cases
    } else {
        if (read(client_socket, &reg, LCLOUD_NET_HEADER_SIZE) != LCLOUD_NET_HEADER_SIZE) {
            return -1;
        }
        if (C0 == LC_POWER_OFF) {
            // Close connection and reset socket
            close(client_socket);
            client_socket = -1;
        }
    }

    // Swap register back to host mode
    reg = ntohll64(reg);
    return(reg);
}