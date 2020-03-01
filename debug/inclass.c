////////////////////////////////////////////////////////////////////////////////
//
//  File           : inclass.c
//  Description    : This is the debugging session function for CMPSC311.
//
//   Author        : Patrick McDaniel
//   Last Modified : Wed Oct 15 10:59:38 EDT 2014
//

// Include Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Includes

// Defines

//
// Global Data

//
// Functional Prototypes

int myfunction( int val );
int mybuffers( void );
int ping( int val );
int pong( int val );

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the SMSA simulator
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main( int argc, char *argv[] ) {

    myfunction( 4 );
    mybuffers();
    pong( 11 );
    ping( 12 );
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : myfunction
// Description  : does a bunch of stuff
//
// Inputs       : val - the initial value of val
// Outputs      : 0 if successful test, -1 if failure

int myfunction( int val ) {

    int i;

    // Walk the ring 7 
    for (i=0; i<10; i++) {
	val = (val + 11) % 7;
	// printf( "Value = %d\n", val );
    }

    // Return successfully
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : mybuffers
// Description  : does a bunch of buffering stuff
//
// Inputs       : none
// Outputs      : 0 if successful test, -1 if failure

int mybuffers( void ) {

    int i, j, seed, ival;
    int z[] = { 7, 11, 13, 17 };
    char *bufs[4]; 

    // Create some buffers
    for (i=0; i<4; i++) {
	bufs[i] = malloc( z[i] );
	memset( bufs[i], 0x1, z[i] );
    }

    // Walk the various prime number rings
    for (i=0; i<20; i++) {
	for (j=0; j<4; j++) {

	    if ( j==3 ) { 
		seed = 21;
	    } else {
		seed = z[j+1];
	    }

	    // Work approximately in ring z[j]
	    ival = (bufs[j][(i-1)%z[j]] + seed) % z[j];
	    bufs[j][i%z[j]] = ival;
	    //printf( "Value bufs[%d][%d] = %d\n", j, i%z[j], bufs[j][i%z[j]] );

	}
    }

    // Walk the list of values (and free them)
    for (i=0; i<4; i++) {
	for (j=0; j<z[i]; j++) {
	    printf( "z[%d][%d] = %d\n", i, j, bufs[i][j] );
	}
	free( bufs[i] );
	//printf( "\n\n" );
    }

    // Return successfully
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : ping
// Description  : the ping side of our code game
//
// Inputs       : value - the current value
// Outputs      : 0 if successful test, -1 if failure

int ping( int val ) {
    printf( "Ping %d\n", val );
    if( val <= 0 ) {
	return( 0 );
    }
    if ( val % 2 == 0 ) {
	pong( val-1 );
    } 
    return( ping( val-1 ) );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : pong
// Description  : the ping side of our code game
//
// Inputs       : value - the current value
// Outputs      : 0 if successful test, -1 if failure

int pong( int val ) {
    printf( "Pong %d\n", val );
    if( val <= 0 ) {
	return( 0 );
    }
    if ( val % 2 == 0 ) {
	ping( val-1 );
    } 
    return( pong( val-1 ) );
}
