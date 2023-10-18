/*
 * File:   io.c
 * Author: szeyu
 *
 * Created on October 10, 2023, 9:04 AM
 */


#include "xc.h"
#include "timer.h"


void IOinit() {
   
    // SET up INPUT Ports
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    
    // SET up OUTPUT Ports
    TRISBbits.TRISB8 = 0;
    
    // Select digital on pin 7
    AD1PCFGbits.PCFG4 = 1;
    
    // Set weak pull up for inputs
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE  = 1;
    CNPU1bits.CN1PUE  = 1;
    
    // Switch clock to 500 kHz
    CLKDIVbits.RCDIV = 0;
    __builtin_write_OSCCONH(0x66);
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN == 1);
    
    T2CONbits.TCKPS = 0b10;
            
    SRbits.IPL = 0;
    
}


void toggleLED()
{
    // ~ operator complements the bit
    
    LATBbits.LATB8 = ~LATBbits.LATB8;
}


void IOcheck() {
    
    uint8_t PB1 = !(PORTAbits.RA2);
    uint8_t PB2 = !(PORTAbits.RA4);
    uint8_t PB3 = !(PORTBbits.RB4);
    
    uint8_t PB = (PB3 << 2) + (PB2 << 1) + PB1;
    
    uint16_t delay;
    uint8_t toggle = 1;
    
    switch (PB)
    {
        
        case 0b001:
            delay = 500;
            break;
        case 0b010:
            delay = 1000;
            break;
        case 0b100:
            delay = 5000;
            break;
        case 0b011:
            delay = 1;
            break;
        default:
            delay = 0;
            toggle = 0;
            break;
            
    }
    
    delay_ms(delay);
    
    if (toggle)
    {
        toggleLED();
    }
    else
    {
        LATBbits.LATB8 = 0;
    }
    
}
