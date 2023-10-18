/*
 * File:   timer.c
 * Author: Steven Ng 30088297
 *
 * Created on October 10, 2023, 9:01 AM
 */

#include "xc.h"

// Configure Timer Registers
void timerInit()
{
    IEC0bits.T2IE = 1; // Enable Timer2 Interrupt
    IFS0bits.T2IF = 0; // Lower Timer2 Interrupt Status Flag
    T2CONbits.T32 = 0; // Set Timer2 to be 16 bit
    T2CONbits.TON = 0; // Turn off timer2
    PR2           = 0; // Set Timer2 Period to 0
    TMR2          = 0; // Set TMR2 to 0;
}

// delay function
void delay_ms(uint16_t time_ms)
{
    // Based on millisecond to wait for, set Timer2 period accordingly
    switch(time_ms)
    {
        // 1ms
        case 1:
            PR2 = 4;
            break;
        
        // 500ms
        case 500:
            PR2 = 1953;
            break;
        
        // 1000ms
        case 1000:
            PR2 = 3906;
            break;
        
        // 5000ms    
        case 5000:
            PR2 = 19531;
            break;
        
        // All other cases PR2 is 0
        default:
            PR2 = 0;
            break;
    }
    
    // If PR2 is set to a non zero value,
    // turn on Timer2 and enter Idle till interrupt
    if (PR2 > 0)
    {
        T2CONbits.TON = 1;
        Idle();
    }
    
}