/*
 * File:   main.c
 * Author: Steven Ng and Aaron Born
 *
 * Created on November 21, 2023, 8:47 AM
 */

#include "xc.h"
#include "UART.h"
#include "string.h"



void InitUART2(void) 
{
	// configures UART2 module on pins RB0 (Tx) and RB1 (Rx) on PIC24F16KA101 
	// Enables UART2 
    
    // TARGET: 4800 baud @ 500 kHz  FOSC

	U2MODEbits.USIDL = 0;	// Bit13 Continue in Idle
	U2MODEbits.IREN = 0;	// Bit12 No IR translation
	U2MODEbits.RTSMD = 0;	// Bit11 Flow Control Mode Mode
	U2MODEbits.UEN = 00;    // Bits8,9 TX,RX enabled, CTS,RTS not
	U2MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
	U2MODEbits.LPBACK = 0;  // Bit6 No Loop Back
	U2MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
	U2MODEbits.RXINV = 0;	// Bit4 Idle state is '1'
	U2MODEbits.BRGH = 1;	// Bit3 16 clocks per bit period
	U2MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
	U2MODEbits.STSEL = 0;	// Bit0 One Stop Bit
    
    U2BRG = 12; //gives a baud rate of 4807.7 Baud with 500kHz clock; Set Baud to 4800 on realterm
 
    U2STAbits.UTXISEL1 = 0;	// Bit15 Int when Char is transferred (1/2 config!)
    U2STAbits.UTXISEL0 = 1;	// Generate interrupt with last character shifted out of U2TXREG buffer
	U2STAbits.UTXINV = 0;	// Bit14 N/A, IRDA config
	U2STAbits.UTXBRK = 0;	// Bit11 Disabled
	U2STAbits.UTXEN = 0;	// Bit10 TX pins controlled by peripheral
//	U2STAbits.UTXBF = 0;	// Bit9 *Read Only Bit*
//	U2STAbits.TRMT = 0;		// Bit8 *Read Only bit*
	U2STAbits.URXISEL = 0;	// Bits6,7 Int. on character recieved
	U2STAbits.ADDEN = 0;	// Bit5 Address Detect Disabled
//	U2STAbits.RIDLE = 0;	// Bit4 *Read Only Bit*
//	U2STAbits.PERR = 0;		// Bit3 *Read Only Bit*
//	U2STAbits.FERR = 0;		// Bit2 *Read Only Bit*
//	U2STAbits.OERR = 0;		// Bit1 *Read Only Bit*
//	U2STAbits.URXDA = 0;	// Bit0 *Read Only Bit*

    IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
    IPC7bits.U2TXIP = 3;    // UART2 TX interrupt has interrupt priority 3-4th highest priority
    
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IPC7bits.U2RXIP = 4;    // UART2 Rx interrupt has 2nd highest priority
    IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// Turn the peripheral on
    AD1PCFGbits.PCFG2 = 1;  // Set AN2 to digital since this will be used for U2TX
    AD1PCFGbits.PCFG3 = 1;  // Set AN3 to digital since this will be used for U2RX

}



// Transmits String of characters to UART
void Disp2String(char *str)
{
    
    unsigned int i;
    
    for (i=0; i<= strlen(str); i++)
    {
        XmitUART2(str[i],1);
    }

    return;
    
}



void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
	IFS1bits.U2TXIF = 0;
}
