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
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware

    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);

    //Shut the Board LED off
    //Board_LED_Set(0, false);
    //Board_LED_Set(1, false);
    //Board_LED_Set(2, false);

#endif
#endif

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    Chip_GPIO_Init(LPC_GPIO);  //turn on GPIOs
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 11);  //Port 2, pin 11 (P2_11) set to out

    while(1) {
     Chip_GPIO_SetPinOutLow(LPC_GPIO, 2, 11);  //Set P2_11 output low
     for (i = 0; i< 262144; i++) { }           // delay like 1/4 second or so
     Chip_GPIO_SetPinOutHigh(LPC_GPIO, 2, 11);  //Set P2_11 output high
     for (i = 0; i< 262144; i++) { }           // delay like 1/4 second or so


        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
