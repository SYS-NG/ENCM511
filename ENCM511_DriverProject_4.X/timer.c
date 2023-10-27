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
    // Set CPU interrupt priority to 7 (disable user interrupt))
    SRbits.IPL = 7;
    
    // Switch clock to 500 kHz
    CLKDIVbits.RCDIV = 0;
    __builtin_write_OSCCONH(0x66);
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN == 1);
    
    // Set 1:64 prescaler on TIMER2
    T1CONbits.TCKPS = 0b10;
    T2CONbits.TCKPS = 0b10;
    
    // Set CPU interrupt priority to 0
    SRbits.IPL = 0;
    
    IEC0bits.T1IE = 1; // Enable Timer1 Interrupt
    IFS0bits.T1IF = 0; // Lower Timer1 Interrupt Status Flag
    T1CONbits.TON = 0; // Turn off timer1
    PR1           = 40;
    TMR1          = 0;
    
    IEC0bits.T2IE = 1; // Enable Timer2 Interrupt
    IFS0bits.T2IF = 0; // Lower Timer2 Interrupt Status Flag
    T2CONbits.T32 = 0; // Set Timer2 to be 16 bit
    T2CONbits.TON = 0; // Turn off timer2
    PR2           = 0; // Set Timer2 Period to 0
    TMR2          = 0; // Set TMR2 to 0;
}