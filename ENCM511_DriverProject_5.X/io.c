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
    AD1CON1bits.ADON  = 1;
    AD1CON1bits.SAMP  = 1;
    AD1CSSL           = 0x0020;
    IEC0bits.AD1IE    = 1;
    IFS0bits.AD1IF    = 0;
}
