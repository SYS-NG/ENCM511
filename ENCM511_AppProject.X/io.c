/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on November 21, 2023, 8:47 AM
 */

#include "xc.h"
#include "timer.h"

void IOinit()
{
    
    // Set up analog input ports
    TRISAbits.TRISA3  = 1;   // Configure AN5 (RA3) as input
    AD1PCFGbits.PCFG5 = 0;   // Configure AN5 as analog
    
    // Set up input ports
    TRISAbits.TRISA2  = 1;
    AD1PCFGbits.PCFG4 = 1;
    TRISAbits.TRISA4  = 1;
    TRISBbits.TRISB4  = 1;
    
    // Set weak pull up for inputs
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE  = 1;
    CNPU1bits.CN1PUE  = 1;
    
    // Enable Input Change Notification Interrupt
    IFS1bits.CNIF = 0;  
    IEC1bits.CNIE = 1;  

    // Enable individual interrupt pins
    CNEN2bits.CN30IE = 1;
    CNEN1bits.CN0IE  = 1;
    CNEN1bits.CN1IE  = 1;
    
    // Set up output port
    TRISBbits.TRISB8 = 0;
    
    // Set up ADC
    AD1CON1bits.FORM   = 0b00;    // Format data as integer
    AD1CON1bits.ASAM   = 0;       // Sampling begins when SAMP bit is set
    AD1CON1bits.SSRC   = 0b111;   // Internal counter to end sampling and start conversion
    AD1CON1bits.ADSIDL = 0;       // Operate ADC when in Idle
    AD1CON1bits.DONE   = 0;       // Clear conversion complete bit

    AD1CON2bits.VCFG   = 0b000;   // Selects AVDD and AVSS as voltage references
    AD1CON2bits.CSCNA  = 0;       // Do not scan inputs when performing ADC conversion
    AD1CON2bits.BUFM   = 0;       // Configure 16-word buffer
    AD1CON2bits.ALTS   = 0;       // Always use MUX A
    AD1CON2bits.OFFCAL = 0;       // Converts to get actual input values
    
    AD1CON3bits.ADRC = 0;         // Use system clock
    AD1CON3bits.SAMC = 0b11111;   // Set auto-sample time to 31Tad
    AD1CON3bits.ADCS = 0b0;       // A/D Conversion clock set to 2Tcy

    // ADC interrupts
    IPC3bits.AD1IP   = 2;   // Set AD1 interrupt priority
    IEC0bits.AD1IE   = 1;   // Enable AD1 interrupt

    // Channels
    AD1CHSbits.CH0NA = 0;        // Channel 0 negative input is Vref-
    AD1CHSbits.CH0SA = 0b0101;   // Channel 0 positive input is AN5
    
    // Turn off ADC 
    AD1CON1bits.ADON = 0;
    
}


