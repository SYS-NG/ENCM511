/*
 * File:   timer.c
 * Author: Steven Ng 30088297
 *
 * Created on October 10, 2023, 9:01 AM
 */

#include "xc.h"

void delay_ms(uint16_t time_ms)
{
    switch(time_ms)
    {
        case 1:
            PR2 = 4;
            break;
            
        case 500:
            PR2 = 1953;
            break;
            
        case 1000:
            PR2 = 3906;
            break;
        
        case 5000:
            PR2 = 19531;
            break;
            
        default:
            PR2 = 0;
            break;
    }
    
    if (PR2 > 0)
    {
        T2CONbits.TON = 1;
    }
    
    Idle();
}