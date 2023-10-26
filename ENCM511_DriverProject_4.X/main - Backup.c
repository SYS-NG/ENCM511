/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on September 28, 2023, 8:47 AM
 */
// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include "xc.h"
#include "io.h"
#include "uart.h"

// Timer2 Interrupt Service Routine
void _ISR _T2Interrupt(void)
{   
    // Disable Timer2 Interrupt and lower interrupt flag
    IEC0bits.T2IE = 0;
    IFS0bits.T2IF = 0;
    
    // Turn off timer2 and reset TMR2 to 0
    T2CONbits.TON = 0;
    TMR2          = 0;
    
    // Enable Timer2 Interrupt
    IEC0bits.T2IE = 1;
}

int main(void) {
    
    // Configure I/O and interrupts
    IOinit();
    timerInit();
    
    // Forever loop
    while(1)
    {
        // Check I/O states and delay
        IOcheck();
    }
    
    return 0;
}

