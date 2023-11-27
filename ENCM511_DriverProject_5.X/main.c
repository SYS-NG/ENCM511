/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on September 28, 2023, 8:47 AM
 */
// FOSCSEL
#pragma config FNOSC = FRCDIV          // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO  = ON              // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD  = NONE         // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS           // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL  = SOSCHP       // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM    = CSECMD       // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
#pragma config ICS      = PGx2         // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include "xc.h"
#include "io.h"
#include "uart.h"


// GLOBAL
uint16_t digitalRatio_g = 0;
char    mode_g         = 'x'; // 'x' = 120, 'd' = 68

// UART Interrupt Service Routine
void _ISR _U2RXInterrupt(void) {
	IFS1bits.U2RXIF = 0;

    char received_char = U2RXREG;
    if ( received_char == 'x' || received_char == 'd' ) {
        mode_g = received_char;
    }
}

void _ISR _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0;
    digitalRatio_g = ADC1BUF0;
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
        uart_send(mode_g, digitalRatio_g);
        // Wait till next interrupt to repeat state logic
        for(uint16_t i = 0; i < 35000; i++);
    }
    
    return 0;
}

