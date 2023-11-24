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
    // Select digital on pin 7
    AD1PCFGbits.PCFG5 = 0;
    AD1CON2bits.VCFG  = 0b000;
    AD1CON3bits.ADRC  = 0;
    AD1CSSL           = 0x0020;
    IEC0bits.AD1IE    = 1;
    IFS0bits.AD1IF    = 0;
    AD1CON1bits.SSRC  = 0b111;
    AD1CON1bits.FORM  = 0;
    AD1CON1bits.ASAM  = 1;
    AD1CON2bits.VCFG  = 0;
    AD1CON2bits.CSCNA = 0;
    AD1CON2bits.SMPI  = 0;
    AD1CON2bits.BUFM  = 0;
    AD1CON2bits.ALTS  = 0;
    AD1CSSL           = 0x0040;
    AD1CHSbits.CH0SB  = 0b0101;
}
