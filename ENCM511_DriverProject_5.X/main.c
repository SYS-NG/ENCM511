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


// GLOBAL
uint8_t digitalRatio_g = 0;
char    mode_g         = 'x';

// UART Interrupt Service Routine
void _ISR _U2RXInterrupt(void) {

	IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 0;

    received_char = U2RXREG;
    if ( received_char == 'x' || received_char == 'd' ) {
        mode_g = received_char;
    }

    IEC1bits.U2RXIE = 1;
}

// Timer1 Interrupt Service Routine
void _ISR _T1Interrupt(void)
{   
    // Disable Timer1 Interrupt and lower interrupt flag
    IEC0bits.T1IE = 0;
    IFS0bits.T1IF = 0;

    // Turn on ADC
    AD1CON1bits.ADON = 1;
    AD1CON1bits.SAMP = 1;

    // Turn off timer1 and reset TMR1 to 0
    TMR1          = 0;
    T1CONbits.TON = 1;
    // Enable Timer1 Interrupt
    IEC0bits.T1IE = 1;
}

void _ISR _ADC1TInterrupt(void)
{
    //Disp2String("UART Interrupt\n\r");
    asm("nop");
    // Disable ADC Interrupt and lower interrupt flag
    IEC0bits.AD1IE = 0;
    IFS0bits.AD1IF = 0;
    
    digitalRatio_g   = ADC1BUF0;
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;

    // Enable ADC interrupts
    IEC1bits.AD1IE = 1;
}

int main(void)
{
    // Configure I/O, interrupts, and UART2
    IOinit();
    timerInit();
    InitUART2();
    
    // Forever loop
    while(1)
    {
        asm("nop");
        uart_send(mode, digitalRatio_g);
        // Wait till next interrupt to repeat state logic
        Idle();
    }
    
    return 0;
}

