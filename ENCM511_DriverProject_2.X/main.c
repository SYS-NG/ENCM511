/*
 * File:   main.c
 * Author: szeyu
 *
 * Created on September 28, 2023, 8:47 AM
 */


#include "xc.h"


uint8_t  state   = 3;
uint8_t  toggle  = 0;
int32_t counter = 0;
int32_t counter_lim = 0;

void configurePIC() {
   
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

uint8_t readInputSetState() {
    
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


void customWait(uint8_t state) {
    
    // Set the counter limit per the current state
    // counter_lim = 50000 produces ~0.5 second delay
    // counter_lim = 100000 produces ~1 second delay
    // counter_lim = 200000 produces ~2 second delay
    // States 3 and 4 are static, therefore set counter_lim to 0

    switch(state)
    {
        case 0:
            counter_lim = 50000;
            break;
            
        case 1:
            counter_lim = 100000;
            break;
            
        case 2:
            counter_lim = 200000;
            break;
            
        default:
            counter_lim = 0;
            break;
    }
    
    
    // Reset counter to counter_lim, then count down to 0 to produce delay
    
    counter = counter_lim;
    
    while(counter > 0) {
        counter--;
    }

}

void toggleLED()
{
    // ~ operator complements the bit
    
    LATBbits.LATB8 = ~LATBbits.LATB8;
}

void _ISR _CNInterrupt(void)
{
    asm("nop");
    
    // Notification on input change (rising or falling edge)
   
    // Set state based on the three input ports
    state = readInputSetState();
    
    // Set initial state conditions based on returned state
    setStateConditions(state);
    
    // Lower Input Change Notification Flag
    IFS1bits.CNIF = 0;
    
}

int main(void) {
    
    // Configure I/O and interrupts
    configurePIC();
    
    // Initial state condition
    state = readInputSetState();
    setStateConditions(state);
    
    // Forever loop
    while(1)
    {

        // Toggle is set based on state during interrupt service routine
        asm("nop");
        
        // Program will stay in function for a duration determined by state
        customWait(state);
        
        // If in one of the dynamic states, toggle the LED on/off
        if(state <= 2)
        { 
            asm("nop");
            toggleLED();
        }
        
        asm("nop");
         
    }
    
    return 0;
}
