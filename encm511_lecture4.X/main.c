/*
 * File:   main.c
 * Author: Steven Ng 30088297
 *
 * Created on September 14, 2023, 8:51 AM
 */

#include "xc.h"

//int  a = 0; // 16-bits
//char b = 0; // 8-bits
//signed int   c = -127; // -32768 to 32767
//unsigned int d = 0;   // 0      to 65535
//long unsigned int x = 0; // 32-bits
//
//uint8_t y = 0; // 8-bit unsigned integer
//int16_t z = 0; // 16-bit signed integer

int main(void) {
    uint16_t x = 35000;
    uint16_t y = 2;
    uint32_t z = 0;
    
    z = (uint32_t)x * (uint32_t)y;
    
    asm("nop");
    
    uint8_t one   = 0x31; // 00110001
    uint8_t two   = 0x99; // 10011001
    uint8_t three = one ^ two;

    // RA0 digital input
    // RB0 digital output

    TRISAbits.TRISA0 = 1;
    TRISBbits.TRISB0 = 0;
    
    while(1) {
        LATBbits.LATB0 = 1;
    }
    
    return 0;
}
