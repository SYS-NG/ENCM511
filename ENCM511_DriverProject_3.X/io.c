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
    
    // Select digital on pin 7
    AD1PCFGbits.PCFG4 = 1;
    
    // Set weak pull up for inputs
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE  = 1;
    CNPU1bits.CN1PUE  = 1;
    
    // Set CPU interrupt priority to 7 (disable user interrupt))
    SRbits.IPL = 7;
    
    // Switch clock to 500 kHz
    CLKDIVbits.RCDIV = 0;
    __builtin_write_OSCCONH(0x66);
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN == 1);
    
    // Set 1:64 prescaler on TIMER2
    T2CONbits.TCKPS = 0b10;
    
    // Set CPU interrupt priority to 0
    SRbits.IPL = 0;
    
}


void toggleLED()
{
    // ~ operator complements the bit
    
    LATBbits.LATB8 = ~LATBbits.LATB8;
}


void IOcheck()
{
    
    // Read digital inputs
    uint8_t PB1 = !(PORTAbits.RA2);
    uint8_t PB2 = !(PORTAbits.RA4);
    uint8_t PB3 = !(PORTBbits.RB4);
    
    // Combine inputs into single "input pin" variable of the form PB<3:1>
    uint8_t PB = (PB3 << 2) + (PB2 << 1) + PB1;
    
    // "delay" will be set depending on input PB
    uint16_t delay;
    
    // "toggle" defaults to 1 meaning the LED will toggle after the delay
    uint8_t toggle = 1;
    
    // Set the value of delay depending on PB
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
    
    // Delay for the specified number of milliseconds
    delay_ms(delay);
    
    // If "toggle" (i.e. if not the nothing state), toggle the LED
    if (toggle)
    {
        toggleLED();
    }
    else
    {
        // If not "toggle", keep the LED off
        LATBbits.LATB8 = 0;
    }
    
}
