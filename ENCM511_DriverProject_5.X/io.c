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
   // Set up analog input Ports
    TRISAbits.TRISA3  = 1;
    AD1PCFGbits.PCFG5 = 0;

    // Configurations
    AD1CON1bits.FORM   = 0b00; //Setting data
    AD1CON1bits.ASAM   = 1; //Sampling begins automatically
    AD1CON1bits.SSRC   = 0b111; //Internal cou
    AD1CON1bits.ADSIDL = 0; //operate ADC in
    AD1CON1bits.DONE   = 0; //clear conversion

    AD1CON2bits.VCFG   = 0b000; // Selects AVD
    AD1CON2bits.CSCNA  = 0; //Do not scan the
    AD1CON2bits.BUFM   = 0; //Bugger configure
    AD1CON2bits.ALTS   = 0; //Always use MUX A
    AD1CON2bits.OFFCAL = 0; //converts actua
    
    AD1CON3bits.ADRC = 0; // Use system cloc
    AD1CON3bits.SAMC = 0b11111; //Setting to
    AD1CON3bits.ADCS = 0b111111;

    // Interrupts
    IPC3bits.AD1IP   = 2; //set interrupt prioity
    IEC0bits.AD1IE   = 1; //enable ADC interrupt

    // Channels
    AD1CHSbits.CH0NA = 0;      //Channel 0 negative
    AD1CHSbits.CH0SA = 0b0101; //channel 
    AD1CHSbits.CH0SB = 0b0101; //channel 
    // Turn on ADC 
    AD1CON1bits.ADON = 1;
}
