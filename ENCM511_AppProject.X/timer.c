/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on November 21, 2023, 8:47 AM
 */

#include "xc.h"

void setTimer(uint16_t timer) {

    if ( (timer != 500) && (timer != 32) && (timer != 8) ) return;

    // Set CPU interrupt priority to 7 (disable user interrupt))
    SRbits.IPL = 7;
    
    // Switch clock to 500 kHz
    CLKDIVbits.RCDIV = 0;
    if ( timer == 32 )
    {
        __builtin_write_OSCCONH(0x55);
    }
    else if ( timer == 8 )
    {
        __builtin_write_OSCCONH(0x77);
    }
    else
    {
        __builtin_write_OSCCONH(0x66);
    }
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN == 1);
    
    // Set CPU interrupt priority to 0
    SRbits.IPL = 0;
}

void delay_ms(uint16_t n)
{
    
    // Calculate PR1 based on specified number of milliseconds
    PR1 = (int) (((float) n / 1000) / ((float) 1 / 3906));
    TMR1          = 0;  // Set TMR1 to 0
    T1CONbits.TON = 1; // Turn timer off to start
    
}

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
    
    // Set CPU interrupt priority to 0
    SRbits.IPL = 0;
    
    // Timer1 configuration
    T1CONbits.TON = 0; // Turn timer off to start
    T1CONbits.TSIDL = 0; // Continue timer operation in idle
    T1CONbits.TCKPS = 0b10; // Set 1:64 prescaler
    T1CONbits.TCS = 0; // Set Timer1 clock source to internal clock
    IFS0bits.T1IF = 0;  // Lower Timer1 interrupt status flag
    IEC0bits.T1IE = 1;  // Enable Timer1 interrupt
    PR1           = 1953; // Approx 0.5s Timer1 cycle
    TMR1          = 0;  // Set TMR1 to 0
    IPC0bits.T1IP = 2;
    
    // Timer2 configuration
    T2CONbits.T32 = 0; // Timer2 and Timer 3 act as two 16-bit timers
    T2CONbits.TON = 0; // Turn timer off to start
    T2CONbits.TSIDL = 0; // Continue timer operation in idle
    T2CONbits.TCKPS = 0b11; // Set 1:256 prescaler
    T2CONbits.TCS = 0; // Set Timer2 clock source to internal clock
    IFS0bits.T2IF = 0;  // Lower Timer2 interrupt status flag
    IEC0bits.T2IE = 1;  // Enable Timer2 interrupt
    PR2           = 4883; // Approx 5s Timer2 cycle
    TMR2          = 0;  // Set TMR2 to 0
    IPC1bits.T2IP = 2;

    // Timer3 configuration
    T3CONbits.TON = 0; // Turn timer off to start
    T3CONbits.TSIDL = 0; // Continue timer operation in idle
    T3CONbits.TCKPS = 0b01; // Set 1:8 prescaler
    T3CONbits.TCS = 0; // Set Timer3 clock source to internal clock
    IFS0bits.T3IF = 0;  // Lower Timer3 interrupt status flag
    IEC0bits.T3IE = 1;  // Enable Timer3 interrupt
    PR3           = 1250; // Approx 40ms Timer3 cycle
    TMR3          = 0;  // Set TMR3 to 0
    IPC2bits.T3IP = 7;
    
    // Reference OSC
    REFOCONbits.ROSSLP = 1;
    REFOCONbits.ROSEL  = 0;
    REFOCONbits.RODIV  = 0;
    REFOCONbits.ROEN   = 1;
}