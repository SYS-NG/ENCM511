/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on September 28, 2023, 8:47 AM
 */
// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include "xc.h"
#include "io.h"
#include "uart.h"

enum State { F0, F1, F2, F3, F4, S0, S1, S2 };
enum State   current_state  = F0;
uint8_t state_change   = 1;
uint8_t previous_PB1   = 0;
uint8_t previous_PB2   = 0;
uint8_t previous_PB3   = 0;
uint8_t PB_action      = 0;

void _ISR _CNInterrupt(void)
{
    Disp2String("Change detected\n\r");
    asm("nop");
    
    // Disable Input Change Notification Interrupt and lower flag
    IEC1bits.CNIE = 0; 
    IFS1bits.CNIF = 0;
    T1CONbits.TON = 1;
}

// Timer1 Interrupt Service Routine
void _ISR _T1Interrupt(void)
{   
    Disp2String("T1 Interrupt\n\r");
    asm("nop");
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
        asm("nop");
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
            else
            {
                state_change = 0;
            }      
        }
        else if (current_state == F2)
        {
            if (trigger == 0b010)
            {
                current_state = F0;
                state_change = 1;
            }
            else
            {
                state_change = 0;
            }      
        }
        else if (current_state == F3)
        {
            if (trigger == 0b001)
            {
                current_state = F0;
                state_change = 1;
            }
            else
            {
                state_change = 0;
            }      
        }
        else if (current_state == F4)
        {
            if (trigger == 0b100)
            {
                current_state = F0;
                state_change = 1;
            }
            else if (trigger == 0b010)
            {
                current_state = F0;
                state_change = 1;
            }
            else if (trigger == 0b001)
            {
                current_state = F0;
                state_change = 1;
            }
            else
            {
                state_change = 0;
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
            else
            {
                state_change = 0;
            }      
        }
        else if (current_state == S2)
        {
            if (trigger == 0b010)
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
    
    previous_PB1 = PB1;
    previous_PB2 = PB2;
    previous_PB3 = PB3;
    
    // Enable CN interrupts
    IEC1bits.CNIE    = 1;
    
    // Turn off timer1 and reset TMR1 to 0
    T1CONbits.TON = 0;
    TMR1          = 0;
    
    // Enable Timer1 Interrupt
    IEC0bits.T1IE = 1;
}

// Timer2 Interrupt Service Routine void _ISR _T1Interrupt(void)
void _ISR _T2Interrupt(void)
{   
    Disp2String("T2 Interrupt\n\r");
    asm("nop");
    // Disable Timer2 Interrupt and lower interrupt flag
    IEC0bits.T2IE = 0;
    IFS0bits.T2IF = 0;
    TMR2          = 0;

    LATBbits.LATB8 = ~LATBbits.LATB8;
    
    // Enable Timer2 Interrupt
    IEC0bits.T2IE = 1;
}

int main(void) {
    
    
    /*
    while(1) {
        XmitUART2('a', 5);
        for(uint32_t i = 0; i < 60000; i++);
    }
     */
    // Configure I/O and interrupts
    IOinit();
    timerInit();
    InitUART2();
    
    char *disp_str;

    // Forever loop
    while(1)
    {
        
        // Based on millisecond to wait for, set Timer2 period accordingly
        // Sample Calculation for 0.25 s:
        //      Timer2 increment frequency = (500kHz)/(2 * 64)
        //                                 = 3906.25 Hz
        //      PR2 = (0.25 s) * (3906.25 Hz)
        //          = 976.56
        //          = 977 
        asm("nop");
        
        if (state_change)
        {
            Disp2String("State Change detected\n\r");
            TMR2 = 0;
            switch (current_state)
            {

                case F1:
                    disp_str = "Fast Mode: PB1 was pressed\n\r";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 977;
                    break;

                case F2:
                    disp_str = "Fast Mode: PB2 was pressed\n\r";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 1953;
                    break;

                case F3:
                    disp_str = "Fast Mode: PB3 was pressed\n\r";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 3906;
                    break;

                case F4:
                    if ( action_PB == 0b110)
                    {
                        disp_str = "Fast Mode: PB1 and PB2 are pressed\n\r";
                    }
                    else if ( action_PB == 0b101 )
                    {
                        disp_str = "Fast Mode: PB1 and PB3 are pressed\n\r";
                    }
                    else
                    {
                        disp_str = "Fast Mode: PB2 and PB3 are pressed\n\r";
                    }

                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 0;
                    break;

                case S0:
                    disp_str = "SLow Mode: IDLE\n\r";
                    LATBbits.LATB8 = 0;
                    T2CONbits.TON  = 0;
                    break;
                    
                case S1:
                    disp_str = "Slow Mode: PB1 was pressed\n\r";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 19531;
                    break;
                    
                case S2:
                    disp_str = "Slow Mode: PB2 was pressed\n\r";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 11719;
                    break;

                default: // F0
                    disp_str = "Fast Mode: IDLE\n\r";
                    LATBbits.LATB8 = 0;
                    T2CONbits.TON  = 0;
                    break;

            }
            Disp2String(disp_str);
            state_change = 0;
            asm("nop");
        }

        Idle();
    }
    
    return 0;
}

