/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on September 28, 2023, 8:47 AM
 */

#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include "xc.h"
#include "io.h"

//void _ISR _CNInterrupt(void) 
void _ISR _T2Interrupt(void)
{   
    IEC0bits.T2IE = 0;
    IFS0bits.T2IF = 0;
   
    T2CONbits.TON = 0;
    TMR2          = 0;
    
    IEC0bits.T2IE = 1;
}

int main(void) {
    
    // Configure I/O and interrupts
    IOinit();
    timerInit();
    
    // Forever loop
    while(1)
    {
        // Toggle is set based on state during interrupt service routine
        asm("nop");
        IOcheck();
    }
    
    return 0;
}

