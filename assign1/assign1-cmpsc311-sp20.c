////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmpsc311-f16-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMPSC311, Spring 2020, at Penn State.
//                   assignment page for details.
//
//   Author        : Hunter Schuler
//   Last Modified : 2/4/2020
//

// Include Files
#include <stdio.h>
#include <string.h>

#define NUM_TEST_ELEMENTS 100

// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : roud311
// Description  : A function to manually round floats to ints
//
// Inputs       : fValue - a float value
//
// Outputs      : returns the value as an int

int round311(float fValue) {
    if (fValue > 0) {
        return (fValue + .5);
    } else {
        return (fValue - .5);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : printFloatArr311
// Description  : A function to print an array of floats
//
// Inputs       : fArray - a pointer to a float array
//                elementCount - the number of items in Array
//
// Outputs      : none

void printFloatArr311(float *fArray, int elementCount) {
    for (int i=0; i < elementCount; i++) {
        if (i == elementCount-1) {
            printf("%.2f\n", fArray[i]);
            return;
        }
        printf("%.2f, ", fArray[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : printIntArr311
// Description  : A function to print an array of integers
//
// Inputs       : iArray - a pointer to an integer array
//                elementCount - the number of items in iArray
//
// Outputs      : none

void printIntArr311(int *iArray, int elementCount) {
    for (int i=0; i < elementCount; i++) {
        if (i == elementCount-1) {
            printf("%d\n", iArray[i]);
            return;
        }
        printf("%d, ", iArray[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : bubbleSort311
// Description  : A function to sort an integer array using the bubble sort algorithm
//
// Inputs       : iArray - a pointer to an integer array
//                elementCount - the number of items in iArray
//
// Outputs      : none

void bubbleSort311(int *iArray, int elementCount) {
    for (int i=0; i < elementCount; i++){
        for (int j=0; j < elementCount-i; j++){
            if(iArray[j] > iArray[j+1]) {
                int temp = iArray[j];
                iArray[j] = iArray[j+1];
                iArray[j+1] = temp;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : toBinary
// Description  : A function to print a char value in binary through bit masking
//
// Inputs       : val - The char value to be printed in binary
//
// Outputs      : none

void toBinary(char val) {
    // Print each value from the char
    // Uses a mask of 0x80 (1000 0000)
    // Mask is shifted right at every loop
    // Uses the bit-wise comparator to check if value is 0 or not 0
    int mask = 0x80;
    for (int i = 0; i < 8; i++){
        printf("%d", (val & mask) > 0);
        if (i == 3) {
            printf(" ");
        }
        mask >>= 1;
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : countRange311
// Description  : A function to return 3*times the count of items in the min and max
//
// Inputs       : fArray - pointer to a float array
//                elementCount - elements in the array passed in
//                min - minimum value testing for in fArray
//                max - maximum value testing for in fArray
//
// Outputs      : count - 3x the count of the items in fArray

int countRange311(float *fArray, int elementCount, int min, int max) {
    int count = 0;
    for (int i = 0; i < elementCount; i++) {
        int rounded = round311(fArray[i]);
        if (rounded >= min && rounded <= max) {
            count += 3;
        }
    }
    return count;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : histogram311
// Description  : The main function for the CMPSC311 assignment #1
//
// Inputs       : histArray - pointer to array
//                size - size of the array passed in
//
// Outputs      : 0 if successful test, -1 if failure

int histogram311(int *histArray, size_t size) {
    // test array size by getting size of array in bytes and dividing by length of its first element
    if (size/sizeof(histArray[0]) != 10) {
        return -1;
    }
    // Loop through each row
    for (int i = 0; i < 23; i++) {
        // Print the first three characters of each row
        if (i == 0 || i > 20) {
            printf("   ");
        } else if (i <= 10) {
            printf("%d |", 20 - i);
        } else {
            printf("%d  |", 20 - i);
        }
        // Loop through each column
        for (int j = 0; j < 40; j++) {
            if (i == 0 || i == 21) {
                printf("+----------------------------------------+");
                j = 45;
            } else if (i % 5 == 0) {
                if ((j == 1 || j == 2) && (histArray[0]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 5 || j == 6) && (histArray[1]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 9 || j == 10) && (histArray[2]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 13 || j == 14) && (histArray[3]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 17 || j == 18) && (histArray[4]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 21 || j == 22) && (histArray[5]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 25 || j == 26) && (histArray[6]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 29 || j == 30) && (histArray[7]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 33 || j == 34) && (histArray[8]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 37 || j == 38) && (histArray[9]/3) >= (20 - i)) {
                    printf("x");
                } else {
                    printf(".");
                }
            } else if (i < 22) {
                if ((j == 1 || j == 2) && (histArray[0]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 5 || j == 6) && (histArray[1]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 9 || j == 10) && (histArray[2]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 13 || j == 14) && (histArray[3]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 17 || j == 18) && (histArray[4]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 21 || j == 22) && (histArray[5]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 25 || j == 26) && (histArray[6]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 29 || j == 30) && (histArray[7]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 33 || j == 34) && (histArray[8]/3) >= (20 - i)) {
                    printf("x");
                } else if ((j == 37 || j == 38) && (histArray[9]/3) >= (20 - i)) {
                    printf("x");
                } else {
                    printf(" ");
                }
            }
        }
        // Print the closing bar of the histogram
        if (i != 0 && i < 21) {
            printf("|");
        }
        if (i == 22) {
            printf("  00  10  20  30  40  50  60  70  80  90 ");
        }
        printf("\n");
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMPSC311 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main(int argc, char **argv) {

    /* Local variables */
    float f_array[NUM_TEST_ELEMENTS];
    int i_array[NUM_TEST_ELEMENTS], i;
    //int hist_array[10];

    /* Preamble inforamtion */
    printf( "CMPSC311 - Assignment #1 - Spring 2020\n\n" );

    /* Step #1 - read in the float numbers to process */
    for (i=0; i<NUM_TEST_ELEMENTS; i++) {
        scanf("%f", &f_array[i]);
    }

    /* Step #2 - Alter the values of the float array as follows: all 
    even numbered indices of the array should be multiplied by 0.78 
    if the value is greater than 50 and 1.04 otherwise.  All odd valued
    indices should multiplied by 0.92 if the value is greater than 50 
    and 1.15 otherwise */
    for (i=0; i<NUM_TEST_ELEMENTS; i++) {
        if (i % 2 == 0) {
            if (f_array[i] > 50) {
                f_array[i] = f_array[i] * .78;
            } else {
                f_array[i] = f_array[i] * 1.04;
            }
        } else {
            if (f_array[i] > 50) {
                f_array[i] = f_array[i] * .92;
            } else {
                f_array[i] = f_array[i] * 1.15;
            }
        }
    }

    /* Step  #3 Round all of the values to integers and assign them 
    to i_array using the round311 function */
    for (i=0; i<NUM_TEST_ELEMENTS; i++) {
        i_array[i] = round311(f_array[i]);
    }

    /* Step #4 - Print out all of the floating point numbers in the 
    array in order on a SINGLE line.  Each value should be printed 
    with 2 digits to the right of the decimal point. */
    printf( "Testing printfloatArr311 (floats): " );
    printFloatArr311(f_array, NUM_TEST_ELEMENTS);
    printf("\n\n");

     /* Step #5 - Print out all of the integer numbers in the array in order on a SINGLE line. */
    printf( "Testing printIntArr311 (integers): " );
    printIntArr311(i_array, NUM_TEST_ELEMENTS);
    printf("\n\n");

    /* Step #6 - sort the integer values, print values */
    printf( "Testing bubbleSort311 (integers): " );
    bubbleSort311(i_array, NUM_TEST_ELEMENTS);
    printIntArr311(i_array, NUM_TEST_ELEMENTS);
    printf("\n\n");

    /* Step #7 - print out the last 5 values of the integer array in binary. */
    printf( "Testing toBinary:\n" );
    for (i=NUM_TEST_ELEMENTS-6; i<NUM_TEST_ELEMENTS; i++) {
        toBinary(i_array[i]);
    }
    printf("\n\n");

    /* Declare an array of integers.  Fill the array with a count (times three) of the number of values for each 
    set of tens within the floating point array, i.e. index 0 will contain the count of rounded values in the array 0-9 TIMES THREE, the
    second will be 10-19, etc. */

    // Declare and initialize the array
    int histArray[10];
    for (i = 0; i < 10; i++) {
        histArray[i] = countRange311(f_array, NUM_TEST_ELEMENTS, 10*i, 10*(i+1)-1);
    }

    if (histogram311(histArray, sizeof(histArray)) == -1) {
        return -1;
    }
    
    /* Exit the program successfully */
    printf( "\n\nCMPSC311 - Assignment #1 - Spring 2020 Complete.\n" );
    return( 0 );
}