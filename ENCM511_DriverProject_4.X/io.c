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
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    
    // SET up OUTPUT Ports
    TRISBbits.TRISB8 = 0;
    
    // Enable Input Change Notification Interrupt
    IEC1bits.CNIE = 1;
    IFS1bits.CNIF = 0;
    
    // Enable individual interrupt pins
    CNEN2bits.CN30IE = 1;
    CNEN1bits.CN0IE  = 1;
    CNEN1bits.CN1IE  = 1;
    
    // Select digital on pin 7
    AD1PCFGbits.PCFG4 = 1;
    
    // Set weak pull up for inputs
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE  = 1;
    CNPU1bits.CN1PUE  = 1;
    
    IPC4bits.CNIP     = 0x3;
}
