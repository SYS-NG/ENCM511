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
State   current_state  = F0;
uint8_t state_change   = 1;
uint8_t previous_PB    = 0;
uint8_t PB_action      = 0;

void _ISR _CNInterrupt(void)
{
    asm("nop");
    
    // Disable Input Change Notification Interrupt and lower flag
    IEC1bits.CNIE = 0; 
    IFS1bits.CNIF = 0;
    T1CONbits.TON = 1;
}

// Timer2 Interrupt Service Routine void _ISR _T1Interrupt(void)
void _ISR _T2Interrupt(void)
{   
    // Disable Timer2 Interrupt and lower interrupt flag
    IEC0bits.T2IE = 0;
    IFS0bits.T2IF = 0;
    TMR2          = 0;

    LATBbits.LATB8 = ~LATBbits.LATB8;
    
    // Enable Timer2 Interrupt
    IEC0bits.T2IE = 1;
}

int main(void) {
    
    // Configure I/O and interrupts
    IOinit();
    timerInit();
    
    const char *disp_str;

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
        
        if (state_change)
        {
            TMR2 = 0;
            switch (current_state)
            {

                case F1:
                    disp_str = "Fast Mode: PB1 was pressed";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 977;
                    break;

                case F2:
                    disp_str = "Fast Mode: PB2 was pressed";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 1953;
                    break;

                case F3:
                    disp_str = "Fast Mode: PB3 was pressed";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 3906;
                    break;

                case F4:
                    disp_str = "Fast Mode: PB_ and PB_ are pressed";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 0;
                    break;

                case S0:
                    disp_str = "SLow Mode: IDLE";
                    LATBbits.LATB8 = 0;
                    T2CONbits.TON  = 0;
                    break;
                    
                case S1:
                    disp_str = "Slow Mode: PB1 was pressed";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 19531;
                    break;
                    
                case S2:
                    disp_str = "Slow Mode: PB2 was pressed";
                    LATBbits.LATB8 = 1;
                    T2CONbits.TON  = 1;
                    PR2            = 11719;
                    break;

                default: // F0
                    disp_str = "Fast Mode: IDLE";
                    LATBbits.LATB8 = 0;
                    T2CONbits.TON  = 0;
                    break;

            }
            Disp2String(disp_str);
            state_change = 0;
        }

        Idle();
    }
    
    return 0;
}

