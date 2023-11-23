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
    TRISAbits.TRISA3 = 1;
    // Select digital on pin 7
    AD1PCFGbits.PCFG5 = 0;

    AD1CSSL = 0x0020;
}
