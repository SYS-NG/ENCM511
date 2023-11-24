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
    
    // Select digital on pin 8
    AD1PCFGbits.PCFG5 = 0;
   
    
//     AD1CON1bits.SSRC  = 0b0111;
//     AD1CON1bits.FORM  = 0;
//     AD1CON1bits.ASAM  = 1;
    
//     AD1CON2bits.VCFG  = 0;
// //    AD1CON2bits.CSCNA = 0;
// //    AD1CON2bits.SMPI  = 0;
// //    AD1CON2bits.BUFM  = 0;
// //    AD1CON2bits.ALTS  = 0;
    
//     AD1CON3bits.ADRC  = 0;
//     AD1CON3bits.ADCS  = 31;
    
//     AD1CSSL           = 0x0020;
    
//     AD1CHSbits.CH0SB  = 0b0101;
/*
    AD1CON1bits.ADON = 1;
    AD1CON1bits.SAMP = 1;

    AD1CON1bits.FORM = 0b00; //Setting data
    AD1CON1bits.ASAM = 0; //Sampling begins
    AD1CON1bits.SSRC = 0b111; //Internal cou

    AD1CON1bits.ADSIDL = 0; //operate ADC in
    AD1CON1bits.DONE  = 0; //clear conversion

    AD1CON2bits.CSCNA = 0; //Do not scan the
    AD1CON2bits.BUFM = 0; //Bugger configure
    AD1CON2bits.ALTS = 0; //Always use MUX A
    AD1CON2bits.VCFG   = 0b000; // Selects AVD
    AD1CON2bits.OFFCAL = 0; //converts actua

    AD1CON3bits.ADRC = 0; // Use system cloc
    AD1CON3bits.SAMC = 0b11111; //Setting to
    AD1CHSbits.CH0NA = 0; //Channel 0 negati

    //AD1CSSLbits.CSSL5 = 0; //analog channe
    IPC3bits.AD1IP = 2; //set interrupt prio
    IEC0bits.AD1IE = 1; //enable ADC interru
    AD1CHSbits.CH0SA = 0b0101; //channel p
    TRISAbits.TRISA3 = 1; //set pin 7 (RA2 /
    AD1PCFGbits.PCFG5 = 0; //set pin 7 (RA2
*/

    // Configure ADC interrupt
    IFS0bits.AD1IF = 0;     // Clear ADC interrupt flag
    IEC0bits.AD1IE = 1;     // Enable ADC interrupt
    IPC3bits.AD1IP = 5;     // Set ADC interrupt priority

    // Configure ADC module
    AD1CON1bits.FORM = 0;   // Data Output Format: Integer
    AD1CON1bits.SSRC = 7;   // Sample Clock Source: Auto-convert
    AD1CON1bits.ASAM = 1;   // Auto-Sampling
//    AD1CON1bits.AD12B = 0;  // 10-bit ADC operation

    AD1CON2bits.VCFG = 0;   // Voltage Reference Configuration: AVdd/AVss
    AD1CON2bits.CSCNA = 0;  // Disable channel scanning
    AD1CON2bits.SMPI = 0;   // Number of samples: 1

    AD1CHSbits.CH0SA = 5;  // Input channel: AN5
    AD1CHSbits.CH0NA = 0;  // Negative input: Vref-

    AD1CON3bits.ADRC = 0;   // Clock Source: System Clock
    AD1CON3bits.SAMC = 16;  // Auto-Sample Time: 16 Tad
    AD1CON3bits.ADCS = 63;  // Conversion Clock: Tad = Tcy

    AD1CON1bits.ADON = 1;   // Enable ADC

}
