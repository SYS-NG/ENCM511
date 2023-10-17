/*
 * File:   timer.c
 * Author: szeyu
 *
 * Created on October 10, 2023, 9:01 AM
 */


#include "xc.h"

void (uint16_t time_ms);
void customWait(uint8_t state) {
    
    // Set the counter limit per the current state
    // counter_lim = 50000 produces ~0.5 second delay
    // counter_lim = 100000 produces ~1 second delay
    // counter_lim = 200000 produces ~2 second delay
    // States 3 and 4 are static, therefore set counter_lim to 0

    switch(state)
    {
        case 0:
            counter_lim = 50000;
            break;
            
        case 1:
            counter_lim = 100000;
            break;
            
        case 2:
            counter_lim = 200000;
            break;
            
        default:
            counter_lim = 0;
            break;
    }
    
    
    // Reset counter to counter_lim, then count down to 0 to produce delay
    
    counter = counter_lim;
    
    while(counter > 0) {
        counter--;
    }

}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);