/*
 * File:   main.c
 * Author: Steven Ng 30088297
 *
 * Created on September 18, 2023, 10:32 PM
 */


#include "xc.h"

// INPUTs RA 0, 1, 2, 4
// User input: RA4, RA2, RA1, RA0 
int input_mask  = 0x0017;

// OUTPUTs RB 0, 1, 2, 4, 7, 8, 9, 12, 13
int output_mask = 0x3397;

uint32_t UCID_nine_digit_g = 300882971;

int setUpPorts() {
    // SET up INPUT Ports
    TRISA = input_mask & 0xFFFF;
    // SET up OUTPUT Ports
    TRISB = ~output_mask & TRISB;
}

// Retrieves the position (starting from 0 and the rightmost bit)
// of the digit to display 
unsigned int retrievePosFromInput() {
    
    // Valid positions from 8 (leftmost and most significant)
    //   to 0 (rightmost and least significant)
    // Based on the input bits values determine the position of the
    //   the target digit
    
    // Start out valid range to account for user input 0000
    unsigned int dig_pos = 9;
        
    if (PORTAbits.RA4) {
        dig_pos -= 8;
    }
    if (PORTAbits.RA2) {
        dig_pos -= 4;
    }
    if (PORTAbits.RA1) {
        dig_pos -= 2;
    }
    if (PORTAbits.RA0) {
        dig_pos -= 1;
    }
    
    return dig_pos;
}

unsigned int getNumBitsToDisplay(unsigned int dig_pos_from_right) {
    
    // Create a variable to be manipulated, initialized with
    //   original UCID value
    uint32_t tmp_val = UCID_nine_digit_g;
    
    // Divide until the digit of interest is in the one's place
    while(tmp_val && (dig_pos_from_right > 0)){
        tmp_val /= 10;
        dig_pos_from_right -= 1;
    }
    
    // Use modulus operation to retrieve digit of interest 
    return tmp_val % 10;
}

unsigned int displayNBits(unsigned int num_bits_display) {
    // Track current output pin
    unsigned int cur_pos = 1;
    // Track all bits that needs to be turned on
    unsigned int on_bits = 0;

    while(num_bits_display > 0) {

        // Move cur_pos to next available LATB bit
        while(!(cur_pos & output_mask)) {
            cur_pos <<= 1;
        }

        on_bits |= cur_pos;

        cur_pos <<= 1;
        num_bits_display -= 1;
    }
    
    // Set LATB to turn on LED pins
    LATB = on_bits;
}



int main(void) {
     
    unsigned int dig_pos_from_right;
    unsigned int num_bits_display;
    
    // Configure IO Pin directions
    setUpPorts();
    
    while(1) {

        dig_pos_from_right = retrievePosFromInput();
        
        // Reset LEDs
        LATB = 0x0000;
        
        if (dig_pos_from_right >= 0 && dig_pos_from_right < 9) {
            
            // Retrieve number of LEDs to turn on from digit in position
            num_bits_display = getNumBitsToDisplay(dig_pos_from_right);
            
            // Turn on number of LEDs
            // For reference: the 9 digit UCID is 300882971
            displayNBits(num_bits_display);
        }
        
        asm("NOP");
    }
    return 0;
}
