/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
   // SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    //Board_LED_Set(0, true);

    // Set the LED to the state of "Off"
    int j=0;


    //This code cycles the onboard LED connected to PIO 2_16/17/18 Green, Red, and Blue in that order
    //Also added a ~250ms delay
    Board_LED_Set(0, true);
    for(j=0;j<262144;j++)
    Board_LED_Set(0, false);
    for(j=0;j<262144;j++)
    Board_LED_Set(1, true);
    for(j=0;j<262144;j++)
    Board_LED_Set(1, false);
    for(j=0;j<262144;j++)
    Board_LED_Set(2, true);
    for(j=0;j<262144;j++)
    Board_LED_Set(2, false);


#endif
#endif

    // TODO: insert code here

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    //int State_Input= 0;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
