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
#include "timer.h"

#define BLINK_LIMIT 100
#define ON          1
#define OFF         0
#define ARR_SIZE    4
#define MAX_PRINT   100

#define AUTH1_PR1   4000 
#define AUTH2_PR1   2000
#define AUTH3_PR1   1000

// Global
int8_t  char_count_g    = -1;
uint8_t u2_index_g      = 0;
char    char_buf_g[ARR_SIZE];
uint8_t blink_count_g   = 0;
uint8_t adc_value_g     = 0;
uint8_t PB_pressed_g    = 0b000;
uint8_t disp_timer_g    = 0; // Timer 2
char    p1_g            = 'K';
char    p2_g            = 'M';
char    p3_g            = 'S';
uint8_t p_num_g         = 2;
uint8_t state_changed_g = 0;
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
enum States state_g = LOCK; // Initial State

// UART Interrupt Service Routine
void _ISR _U2RXInterrupt(void)
{
	
    // Disable UART2 Interrupt and lower interrupt flag
    IEC1bits.U2RXIE = 0;
    IFS1bits.U2RXIF = 0;

    // Read receive buffer until ENTER or end of buffer
    if(U2STAbits.OERR == 1)
    {
        U2STAbits.OERR = 0;
        u2_index_g = 0;
        Disp2String("String too long, please try again!");
    }    
    else if (U2STAbits.URXDA)
    {
        
        char received_char = U2RXREG;
        
        if(received_char == 0x0D)
        {
            
            if(u2_index_g >= 0)
            {
                char_buf_g[u2_index_g] = '\0';
                char_count_g = u2_index_g;
            }
            
            u2_index_g = 0;
            
        }
        else if (u2_index_g >= 3)
        {
            char_count_g = -1;
            u2_index_g = 0;
            Disp2String("String too long, please try again!");
        }
        else
        {
            char_buf_g[u2_index_g] = received_char;
            u2_index_g += 1;
        }
        
    }
    
    // Ensure char buffer terminates with null
    char_buf_g[3] = '\0';
   
    // Enable UART2 Interrupt
    IEC1bits.U2RXIE = 1;
    
}



// ADC1 Interrupt Service Routine
void _ISR _ADC1Interrupt(void)
{
    
    // Disable AD1 Interrupt and lower interrupt flag
    IEC0bits.AD1IE   = 0;
    IFS0bits.AD1IF = 0;
    
    // Turn off ADC and stop sampling process
    AD1CON1bits.ADON = 0;
    AD1CON1bits.SAMP = 0;
    
    // Read digitally coded ADC output from buffer
    // Integer division by 103 gives value from 0-9
    adc_value_g = ADC1BUF0 / 103;
    
    // Enable AD1 Interrupt
    IEC0bits.AD1IE   = 1;
    
    // Turn on ADC and start sampling process
    AD1CON1bits.ADON = 1;
    AD1CON1bits.SAMP = 1;
    
}



// Timer1 Interrupt Service Routine
void _ISR _T1Interrupt(void)
{   
    
    // Disable Timer1 interrupt and lower interrupt flag
    IEC0bits.T1IE = 0;
    IFS0bits.T1IF = 0;
    
    // Turn off Timer1
    T1CONbits.TON = 0;
    
    // Toggle LED and increment counter
    LATBbits.LATB8 = ~LATBbits.LATB8;
    blink_count_g += 1;
    
    // Enable Timer1 Interrupt
    IEC0bits.T1IE = 1;
    
    // Reset TMR1 to 0 and turn on Timer1
    TMR1          = 0;
    T1CONbits.TON = 1;
    
}



// Timer2 Interrupt Service Routine
void _ISR _T2Interrupt(void)
{   
    
    // Disable Timer2 interrupt and lower interrupt flag
    IEC0bits.T2IE = 0;
    IFS0bits.T2IF = 0;
    
    // Turn off Timer2 and reset TMR2 to 0
    T2CONbits.TON = 0;
    TMR2 = 0;

    // Set global display timer flag
    disp_timer_g = 1;
    
    // Enable Timer2 interrupt
    IEC0bits.T2IE = 1;
    
}



// Timer3 Interrupt Service Routine
void _ISR _T3Interrupt(void)
{   
    
    // Disable Timer3 interrupt and lower interrupt flag
    IEC0bits.T3IE = 0;
    IFS0bits.T3IF = 0;
    
    // Turn off Timer3 and reset TMR3 to 0
    T3CONbits.TON = 0;
    TMR3 = 0;
    
    // Read digital input pins
    uint8_t PB1 = !(PORTAbits.RA2);
    uint8_t PB2 = !(PORTBbits.RB4);
    uint8_t PB3 = !(PORTAbits.RA4);
    
    PB_pressed_g = (PB1 << 2) + (PB2 << 1) + PB3;
    
    // Re-enable CN interrupts
    IEC1bits.CNIE = 1;

    // Enable Timer3 Interrupt
    IEC0bits.T3IE = 1;
    
}



void _ISR _CNInterrupt(void)
{
    
    // Disable CN interrupt and lower interrupt flag
    IEC1bits.CNIE = 0;
    IFS1bits.CNIF = 0;
    
    // Start Timer3
    TMR3 = 0;
    T3CONbits.TON = 1;
    
}

// Function to set initial state conditions
void setInitialCondition(uint8_t timer1_val, uint8_t timer2_on, uint8_t cn_on, uint8_t adc_on)
{
    TMR1 = 0;
    TMR2 = 0;
    TMR3 = 0;
    blink_count_g = 0;
    PB_pressed_g  = 0b000;

    disp_timer_g = 0;
    char_count_g = -1;
    for (int i = 0; i < ARR_SIZE; i++)
    {
        char_buf_g[i] = '\0';
    } 

    if ( timer1_val > 0 )
    {
        PR1           = timer1_val;
        T1CONbits.TON = 1;  // Turn on timer1
    }
    else
    {
        T1CONbits.TON = 0;  // Turn off timer1
    }

    if ( timer2_on > 0 )
    {
        T2CONbits.TON = 1;  // Turn on timer2
    }
    else
    {
        T2CONbits.TON = 0;  // Turn off timer2
    }

    if ( cn_on > 0 )
    {
        // Enable Input Change Notification Interrupt
        IEC1bits.CNIE = 1;
        T3CONbits.TON = 1;  // Turn on timer3
    }
    else
    {
        // Disable Input Change Notification Interrupt
        IEC1bits.CNIE = 0;
        T3CONbits.TON = 0;  // Turn off timer3
    }

}

void do_something_uart(char* str)
{
    uint8_t i           = 0;
    uint8_t reached_eos = 0;

    char buffer[MAX_PRINT + 1];
    if ( *str == '\0' ) reached_eos = 1;

    while( i < MAX_PRINT )
    {
        if ( reached_eos == 1)
        {
            buffer[i] = ' ';
        }
        else
        {
            buffer[i] = *str;
            str++;
            if ( *str == '\0' ) reached_eos = 1;
        }
    }

    Disp2String(buffer);
}

int main(void)
{
    // Configure I/O, timers, and UART2
    IOinit();
    timerInit();
    InitUART2();

    while (1) {
        // 0
        if (state_g == LOCK) {
            setInitialCondition(OFF, OFF, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (char_count_g == 1) && (char_buf_g[0] == p1_g) )
                {
                    state_g         = AUTH1;
                    state_changed_g = 1;
                }
                else if ( (char_count_g > -1) && ( (char_count_g != 1) || (char_buf_g[0] != p1_g) ) )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
            } 
        }

        // 1
        if (state_g == AUTH1) {
            setInitialCondition(AUTH1_PR1, OFF, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (char_count_g == 1) && (char_buf_g[0] == p2_g) )
                {
                    state_g         = AUTH2;
                    state_changed_g = 1;
                }
                else if ( (char_count_g > -1) && ( (char_count_g != 1) || (char_buf_g[0] != p2_g) ) )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
                else if ( blink_count_g == BLINK_LIMIT )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
            } 
        }

        // 2
        if (state_g == AUTH2) {
            setInitialCondition(AUTH2_PR1, OFF, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (char_count_g == 1) && (char_buf_g[0] == p3_g) )
                {
                    state_g         = AUTH3;
                    state_changed_g = 1;
                }
                else if ( (char_count_g > -1) && ( (char_count_g != 1) || (char_buf_g[0] != p3_g) ) )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
                else if ( blink_count_g == BLINK_LIMIT )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
            } 
        }

        // 3
        if (state_g == AUTH3) {
            setInitialCondition(AUTH3_PR1, OFF, OFF, ON);
            while (!state_changed_g)
            {
                Idle();
                if ( (PB_pressed_g == 0b010) && (adc_value_g == p_num_g) )
                {
                    state_g         = UNLOCK;
                    state_changed_g = 1;
                }
                else if ( (PB_pressed_g == 0b010) && (adc_value_g != p_num_g)  )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
                else if ( blink_count_g == BLINK_LIMIT )
                {
                    state_g         = ERR_PASS;
                    state_changed_g = 1;
                }
            } 
        }

        // 4
        if (state_g == ERR_PASS) {
            setInitialCondition(OFF, OFF, ON, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (PB_pressed_g == 0b010) )
                {
                    state_g         = LOCK;
                    state_changed_g = 1;
                }
            }
        }

        // 5
        if (state_g == UNLOCK) {
            setInitialCondition(OFF, OFF, ON, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (PB_pressed_g == 0b100) )
                {
                    state_g         = DISP_LOCK;
                    state_changed_g = 1;
                }
                else if ( (PB_pressed_g == 0b010) )
                {
                    state_g         = CHNG_CHAR;
                    state_changed_g = 1;
                }
                else if ( (PB_pressed_g == 0b001) )
                {
                    state_g         = MENU;
                    state_changed_g = 1;
                }
            }
        }

        // 6
        if (state_g == DISP_LOCK) {
            setInitialCondition(OFF, ON, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (disp_timer_g == 1) )
                {
                    state_g         = LOCK;
                    state_changed_g = 1;
                }
            }
        }

        // 7
        if (state_g == CHNG_CHAR) {
            setInitialCondition(OFF, OFF, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (char_count_g == 3) )
                {
                    p1_g = char_buf_g[0];
                    p2_g = char_buf_g[1];
                    p3_g = char_buf_g[2];
                    
                    state_g         = CHNG_NUM;
                    state_changed_g = 1;
                }
                else if ( (char_count_g > -1)  )
                {
                    do_something_uart("Fuck off and try again");
                }
            } 
        }

        // 8
        if (state_g == CHNG_NUM) {
            setInitialCondition(OFF, OFF, ON, ON);
            while (!state_changed_g)
            {
                Idle();
                // Potential concern, state change and immediate update num
                if ( (PB_pressed_g == 0b010) )
                {
                    p_num_g         = adc_value_g;
                    state_g         = DISP_CHNG;
                    state_changed_g = 1;
                }
            } 
        }

        // 9
        if (state_g == DISP_CHNG) {
            setInitialCondition(OFF, ON, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( disp_timer_g == 1 )
                {
                    state_g         = LOCK;
                    state_changed_g = 1;
                }
            }
        }

        // 10
        if (state_g == MENU) {
            setInitialCondition(OFF, OFF, ON, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( PB_pressed_g == 0b110 )
                {
                    state_g         = DISP_IDLE;
                    state_changed_g = 1;
                }
                else if ( PB_pressed_g == 0b100 )
                {
                    state_g         = CHNG_CLK;
                    state_changed_g = 1;
                }
                else if ( PB_pressed_g == 0b010 )
                {
                    state_g         = GAME;
                    state_changed_g = 1;
                }
                else if ( PB_pressed_g == 0b001 )
                {
                    state_g         = UNLOCK;
                    state_changed_g = 1;
                }
            }
        }

        // 11
        if (state_g == CHNG_CLK) {
            setInitialCondition(OFF, OFF, ON, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( PB_pressed_g == 0b100 )
                {
                    setTimer(500);
                    state_g         = MENU;
                    state_changed_g = 1;
                }
                else if ( PB_pressed_g == 0b010 )
                {
                    setTimer(8);
                }
                else if ( PB_pressed_g == 0b001 )  
                {
                    setTimer(500);
                }
            }
        }

        // 12
        if (state_g == GAME) {
            setInitialCondition(OFF, OFF, ON, ON);
            while (!state_changed_g)
            {
                // GAME LOGIC
                Idle();
                if ( PB_pressed_g == 0b010 )
                {
                    state_g         = MENU;
                    state_changed_g = 1;
                }
            }
        }

        // 13
        if (state_g == DISP_IDLE) {
            setInitialCondition(OFF, ON, OFF, OFF);
            while (!state_changed_g)
            {
                Idle();
                if ( (disp_timer_g == 1) )
                {
                    state_g         = LOCK;
                    state_changed_g = 1;
                }
            }
        }
    } // while (1)

    return 0;
}

