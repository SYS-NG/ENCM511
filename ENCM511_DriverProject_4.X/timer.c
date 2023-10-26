/*
 * File:   timer.c
 * Author: Steven Ng 30088297
 *
 * Created on October 10, 2023, 9:01 AM
 */

#include "xc.h"

// Configure Timer Registers
void timerInit()
{
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
    
    IEC0bits.T1IE = 1; // Enable Timer2 Interrupt
    IFS0bits.T1IF = 0; // Lower Timer2 Interrupt Status Flag
    T2CONbits.TON = 0; // Turn off timer2
    
    IEC0bits.T2IE = 1; // Enable Timer2 Interrupt
    IFS0bits.T2IF = 0; // Lower Timer2 Interrupt Status Flag
    T2CONbits.T32 = 0; // Set Timer2 to be 16 bit
    T2CONbits.TON = 0; // Turn off timer2
    PR2           = 0; // Set Timer2 Period to 0
    TMR2          = 0; // Set TMR2 to 0;
}



// Timer1 Interrupt Service Routine
void _ISR _T1Interrupt(void)
{   
    // Disable Timer1 Interrupt and lower interrupt flag
    IEC0bits.T1IE = 0;
    IFS0bits.T1IF = 0;
    
    
    // Read PB states
    
    uint8_t PB1 = !(PORTAbits.RA2);
    uint8_t PB2 = !(PORTAbits.RA4);
    uint8_t PB3 = !(PORTBbits.RB4);
    
    
    // Compare with previous PB states to obtain trigger
    
    uint8_t trigger = 0;
    
    if ((PB1 == 0) && (previous_PB1 == 1))
    {
        trigger += 1 << 2;
    }
    
    if ((PB2 == 0) && (previous_PB2 == 1))
    {
        trigger += 1 << 1;
    }
    
    if ((PB3 == 0) && (previous_PB3 == 1))
    {
        trigger += 1 << 0;
    }
    
    
    // If a button was released, process state transition
    
    if (trigger > 0)
    {
        // Fast mode states
        
        if (current_state == F0)
        {
            if (trigger == 0b100)
            {
                current_state = F1;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = F2;
                state_change = 1;
            }
            else if (trigger == 0b001)
            {
                current_state = F3;
                state_change = 1;
            }
            else if (trigger == 0b111)
            {
                // Transition from fast to slow mode
                current_state = S0;
                state_change = 1;
            }
            else
            {
                current_state = F4;
                state_change = 1;
            }      
        }
        else if (current_state == F1)
        {
            if (trigger == 0b100)
            {
                current_state = F0;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = F2;
                state_change = 1;
            }
            else if (trigger == 0b001)
            {
                current_state = F3;
                state_change = 1;
            }
            else if (trigger == 0b111)
            {
                state_change = 0;
            }
            else
            {
                current_state = F4;
                state_change = 1;
            }      
        }
        else if (current_state == F2)
        {
            if (trigger == 0b100)
            {
                current_state = F1;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = F0;
                state_change = 1;
            }
            else if (trigger == 0b001)
            {
                current_state = F3;
                state_change = 1;
            }
            else if (trigger == 0b111)
            {
                state_change = 0;
            }
            else
            {
                current_state = F4;
                state_change = 1;
            }      
        }
        else if (current_state == F3)
        {
            if (trigger == 0b100)
            {
                current_state = F1;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = F2;
                state_change = 1;
            }
            else if (trigger == 0b001)
            {
                current_state = F0;
                state_change = 1;
            }
            else if (trigger == 0b111)
            {
                state_change = 0;
            }
            else
            {
                current_state = F4;
                state_change = 1;
            }      
        }
        if (current_state == F4)
        {
            if (trigger == 0b100)
            {
                current_state = F1;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = F2;
                state_change = 1;
            }
            else if (trigger == 0b001)
            {
                current_state = F3;
                state_change = 1;
            }
            else if (trigger == 0b111)
            {
                state_change = 0;
            }
            else
            {
                current_state = F0;
                state_change = 1;
            }      
        }
        
        
        // Slow mode states
        
        else if (current_state == S0)
        {
            if (trigger == 0b100)
            {
                current_state = S1;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = S2;
                state_change = 1;
            }
            else if (trigger == 0b111)
            {
                // Transition from slow to fast mode
                current_state = F0;
                state_change = 1;
            }
            else
            {
                state_change = 0;
            }      
        }
        else if (current_state == S1)
        {
            if (trigger == 0b100)
            {
                current_state = S0;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = S2;
                state_change = 1;
            }
            else
            {
                state_change = 0;
            }      
        }
        else if (current_state == S2)
        {
            if (trigger == 0b100)
            {
                current_state = S1;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = S0;
                state_change = 1;
            }
            else
            {
                state_change = 0;
            }      
        }
        else
        {
            // By default, assume no state transition occurs
            state_change = 0;
        }
        
    }
    
    
    // Enable CN interrupts
    IEC1bits.CNIE    = 1;
    CNEN2bits.CN30IE = 1;
    CNEN1bits.CN0IE  = 1;
    CNEN1bits.CN1IE  = 1;
    
    // Turn off timer1 and reset TMR1 to 0
    T1CONbits.TON = 0;
    TMR1          = 0;
    
    // Enable Timer1 Interrupt
    IEC0bits.T1IE = 1;
}
