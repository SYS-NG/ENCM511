/*
 * File:   io.c
 * Author: szeyu
 *
 * Created on October 10, 2023, 9:04 AM
 */


#include "xc.h"

void IOinit() {
   
    // SET up INPUT Ports
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    
    // SET up OUTPUT Ports
    TRISBbits.TRISB8 = 0;
        
    // Enable Input Change Notification Interrupt
    IEC1bits.CNIE = 1;
    
    // Enable individual interrupt pins
    CNEN2bits.CN30IE = 1;
    CNEN1bits.CN0IE  = 1;
    CNEN1bits.CN1IE  = 1;
    
    // Set Input Change Notification Flag to 0
    IFS1bits.CNIF = 0;
    
    // Select digital on pin 7
    AD1PCFGbits.PCFG4 = 1;
    
    // Set weak pull up for inputs
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE  = 1;
    CNPU1bits.CN1PUE  = 1;
}

uint8_t IOcheck() {
    
    uint8_t PB1 = !(PORTAbits.RA2);
    uint8_t PB2 = !(PORTAbits.RA4);
    uint8_t PB3 = !(PORTBbits.RB4);
    
    if ((PB1 && !PB2 && !PB3) || (!PB1 && PB2 && !PB3) || (!PB1 && !PB2 && PB3)) {
        
        // If only one of three buttons is actively pressed...
        if(PB1)
            return 0;
        else if(PB2)
            return 1;
        else if(PB3)
            return 2;
        
    } else if (!(PB1 || PB2 || PB3)) {
        
        // If no buttons are actively pressed...
        return 3;
        
    } else {
        
        // More than one buttons are actively pressed
        return 4;
        
    }
}

void setStateConditions(uint8_t state) {
    
    // For states 0-2 (dynamic states), reset LATB8 because
    // the LED will toggle back on once arriving back in main()
    
    // For states 3 and 4 (static states), set/reset the LED. 
    // LED will not be toggled again until entering a dynamic state
    
    switch(state)
    {
        case 0:
            LATBbits.LATB8 = 0;
            break;
            
        case 1:
            LATBbits.LATB8 = 0;
            break;
            
        case 2:
            LATBbits.LATB8 = 0;
            break;

        case 3:
            LATBbits.LATB8 = 0;
            break;
            
        case 4:
            LATBbits.LATB8 = 1;
            break;
            
    }
    
    counter = 0;
}

void toggleLED() {
    // ~ operator complements the bit
    
    LATBbits.LATB8 = ~LATBbits.LATB8;
}