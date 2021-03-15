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
    // Enter an infinite loop, just incrementing a counter

    // This code reads the pin state of PIO 1_24
    // If it's a digital high, the board LED turns Green
    // If low, the board LED turns red
    while(1) {
        if(Chip_GPIO_GetPinState(LPC_GPIO,1,24)==1){
        	Board_LED_Set(0, true);
        	Board_LED_Set(1, false);
        	Board_LED_Set(2, false);
        }
        else if(Chip_GPIO_GetPinState(LPC_GPIO,1,24)==0) {
        	Board_LED_Set(0, false);
        	Board_LED_Set(1, true);
        	Board_LED_Set(2, false);
        }
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
