/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on September 28, 2023, 8:47 AM
 */


#include "xc.h"
#include "timer.h"
#include "io.h"

uint8_t  state   = 3;
uint8_t  toggle  = 0;
int32_t counter = 0;
int32_t counter_lim = 0;

void _ISR _CNInterrupt(void)
{
    asm("nop");
    
    // Notification on input change (rising or falling edge)
   
    // Set state based on the three input ports
    state = readInputSetState();
    
    // Set initial state conditions based on returned state
    setStateConditions(state);
    
    // Lower Input Change Notification Flag
    IFS1bits.CNIF = 0;
    
}

int main(void) {
    
    // Configure I/O and interrupts
    configurePIC();
    
    // Initial state condition
    state = readInputSetState();
    setStateConditions(state);
    
    // Forever loop
    while(1)
    {

        // Toggle is set based on state during interrupt service routine
        asm("nop");
        
        // Program will stay in function for a duration determined by state
        customWait(state);
        
        // If in one of the dynamic states, toggle the LED on/off
        if(state <= 2)
        { 
            asm("nop");
            toggleLED();
        }
        
        asm("nop");
         
    }
    
    return 0;
}

