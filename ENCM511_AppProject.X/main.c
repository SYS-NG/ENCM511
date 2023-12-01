/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on November 21, 2023, 8:47 AM
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

// Global
int8_t  char_count_g  = -1;
char    char_buf_g[4];
uint8_t blink_count_g = 0;
uint8_t adc_value_g   = 0;
uint8_t PB1_pressed_g = 0;
uint8_t PB2_pressed_g = 0;
uint8_t PB3_pressed_g = 0;
uint8_t disp_timer_g  = 1; // Timer 2
char    p1_g          = 'K';
char    p2_g          = 'M';
char    p3_g          = 'S';
uint8_t p_num_g       = 2;
enum    States { LOCK,
                 AUTH1,
                 AUTH2, 
                 AUTH3,
                 ERR_PASS, 
                 UNLOCK, 
                 DISP_LOCK, 
                 CHNG_CHAR,
                 CHNG_NUM,
                 DISP_CHNG,
                 MENU,
                 CHNG_CLK,
                 GAME,
                 DISP_IDLE,
               };

enum States state_g = LOCK; 

// UART Interrupt Service Routine
void _ISR _U2RXInterrupt(void)
{
	
    // Disable UART2 Interrupt and lower interrupt flag
    IEC1bits.U2RXIE = 0;
    IFS1bits.U2RXIF = 0;

    // Read from receive buffer, and set mode accordingly
    char received_char = U2RXREG;
    if ( received_char == 'x' || received_char == 'd' ) {
        mode_g = received_char;
    }
    
    // Enable UART2 Interrupt
    IEC1bits.U2RXIE = 1;
    
}

// ADC1 Interrupt Service Routine
void _ISR _ADC1Interrupt(void)
{
    
    // Disable AD1 Interrupt and lower interrupt flag
    IEC0bits.AD1IE   = 0;
    IFS0bits.AD1IF = 0;
    
    // Turn off sampling process and turn off ADC
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;
    
    // Read digitally coded ADC output from buffer
    digitalRatio_g = ADC1BUF0;
    
    // Raise ADCFlag to indicate updated ADC value
    ADCFlag = 1;

    // Turn timer 1 back on and reset TMR1 to 0
    T1CONbits.TON = 1;
    TMR1          = 0;
    
    // Enable AD1 Interrupt
    IEC0bits.AD1IE   = 1;
    
}

// Timer1 Interrupt Service Routine
void _ISR _T1Interrupt(void)
{   
    
    // Disable Timer1 Interrupt and lower interrupt flag
    IEC0bits.T1IE = 0;
    IFS0bits.T1IF = 0;
    
    // Turn on ADC and start sampling process
    AD1CON1bits.ADON = 1;
    AD1CON1bits.SAMP = 1;
    
    // Turn off timer1 and reset TMR1 to 0
    T1CONbits.TON = 0;
    TMR1          = 0;
    
    // Enable Timer1 Interrupt
    IEC0bits.T1IE = 1;
    
}


int main(void)
{
    // Configure I/O, timers, and UART2
    IOinit();
    timerInit();
    InitUART2();
    
    // 0
    if (state_g == LOCK) {
        // set initial cond
        while (!()) idle();
        if (true) {

        }
    }
    // 1
    if (state_g == AUTH1) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 2
    if (state_g == AUTH2) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 3
    if (state_g == AUTH3) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 4
    if (state_g == ERR_PASS) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 5
    if (state_g == UNLOCK) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 6
    if (state_g == DISP_LOCK) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 7
    if (state_g == CHNG_CHAR) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 8
    if (state_g == CHNG_NUM) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 9
    if (state_g == DISP_CHNG) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 10
    if (state_g == MENU) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 11
    if (state_g == CHNG_CLK) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 12
    if (state_g == GAME) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    // 13
    if (state_g == DISP_IDLE) {
        // set initial cond
        while (!false) idle();
        if (true) {
            
        }
    }
    
    return 0;
}

