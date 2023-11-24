/*
 * File:   io.c
 * Author: szeyu
 *
 * Created on October 10, 2023, 9:04 AM
 */


#include "xc.h"
#include "timer.h"


void IOinit()
{
    // SET up INPUT Ports
    TRISAbits.TRISA3  = 1;
    AD1PCFGbits.PCFG5 = 0;

// AD1CON2bits.VCFG  = 0b000;
// AD1CON3bits.ADRC  = 0;
// AD1CSSL           = 0x0020;
// IEC0bits.AD1IE    = 1;
// IFS0bits.AD1IF    = 0;
// AD1CON1bits.SSRC  = 0b111;
// AD1CON1bits.FORM  = 0;
// AD1CON1bits.ASAM  = 0;
// AD1CON2bits.VCFG  = 0;
// AD1CON2bits.CSCNA = 0;
// AD1CON2bits.SMPI  = 0;
// AD1CON2bits.BUFM  = 0;
// AD1CON2bits.ALTS  = 0;
// AD1CSSL           = 0x0040;
// AD1CHSbits.CH0SB  = 0b0101;


// Configure ADC interrupt
    IFS0bits.AD1IF = 0;     // Clear ADC interrupt flag
    IEC0bits.AD1IE = 1;     // Enable ADC interrupt
    IPC3bits.AD1IP = 5;     // Set ADC interrupt priority

    // Configure ADC module
    AD1CON1bits.FORM = 0;   // Data Output Format: Integer
    AD1CON1bits.SSRC = 7;   // Sample Clock Source: Auto-convert
    AD1CON1bits.ASAM = 1;   // Auto-Sampling

    AD1CON2bits.VCFG  = 0;   // Voltage Reference Configuration: AVdd/AVss
    AD1CON2bits.CSCNA = 0;  // Disable channel scanning
    AD1CON2bits.SMPI  = 12;   // Number of samples: 1

    AD1CHSbits.CH0SA = 5;  // Input channel: AN5
    AD1CHSbits.CH0NA = 0;  // Negative input: Vref-

    AD1CON3bits.ADRC = 0;   // Clock Source: System Clock
    AD1CON3bits.SAMC = 16;  // Auto-Sample Time: 16 Tad
    AD1CON3bits.ADCS = 63;  // Conversion Clock: Tad = Tcy

    AD1CON1bits.ADON = 1;   // Enable ADC
}
