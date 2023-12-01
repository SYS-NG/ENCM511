/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on November 21, 2023, 8:47 AM
 */

#include "xc.h"
#include "timer.h"



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



void IOinit()
{
    
    // Set up analog input ports
    TRISAbits.TRISA3  = 1;   // Configure AN5 (RA3) as input
    AD1PCFGbits.PCFG5 = 0;   // Configure AN5 as analog
    
    // Set up input ports
    TRISAbits.TRISA2  = 1;
    AD1PCFGbits.PCFG4 = 1;
    TRISAbits.TRISA4  = 1;
    TRISBbits.TRISB4  = 1;
    
    // Set weak pull up for inputs
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE  = 1;
    CNPU1bits.CN1PUE  = 1;
    
    // Enable Input Change Notification Interrupt
    IFS1bits.CNIF = 0;  
    IEC1bits.CNIE = 1;  

    // Enable individual interrupt pins
    CNEN2bits.CN30IE = 1;
    CNEN1bits.CN0IE  = 1;
    CNEN1bits.CN1IE  = 1;
    
    // Set up output port
    TRISBbits.TRISB8 = 0;
    
    // Set up ADC
    AD1CON1bits.FORM   = 0b00;    // Format data as integer
    AD1CON1bits.ASAM   = 0;       // Sampling begins when SAMP bit is set
    AD1CON1bits.SSRC   = 0b111;   // Internal counter to end sampling and start conversion
    AD1CON1bits.ADSIDL = 0;       // Operate ADC when in Idle
    AD1CON1bits.DONE   = 0;       // Clear conversion complete bit

    AD1CON2bits.VCFG   = 0b000;   // Selects AVDD and AVSS as voltage references
    AD1CON2bits.CSCNA  = 0;       // Do not scan inputs when performing ADC conversion
    AD1CON2bits.BUFM   = 0;       // Configure 16-word buffer
    AD1CON2bits.ALTS   = 0;       // Always use MUX A
    AD1CON2bits.OFFCAL = 0;       // Converts to get actual input values
    
    AD1CON3bits.ADRC = 0;         // Use system clock
    AD1CON3bits.SAMC = 0b11111;   // Set auto-sample time to 31Tad
    AD1CON3bits.ADCS = 0b0;       // A/D Conversion clock set to 2Tcy

    // Interrupts
    IPC3bits.AD1IP   = 2;   // Set AD1 interrupt priority
    IEC0bits.AD1IE   = 1;   // Enable AD1 interrupt

    // Channels
    AD1CHSbits.CH0NA = 0;        // Channel 0 negative input is Vref-
    AD1CHSbits.CH0SA = 0b0101;   // Channel 0 positive input is AN5
    AD1CHSbits.CH0SB = 0b0101;   // Channel B positive input is AN5
    
    // Turn off ADC 
    AD1CON1bits.ADON = 0;
    
}


