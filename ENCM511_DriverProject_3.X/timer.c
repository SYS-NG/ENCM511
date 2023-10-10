/*
 * File:   timer.c
 * Author: szeyu
 *
 * Created on October 10, 2023, 9:01 AM
 */


#include "xc.h"

void delay_ms(uint16_t time_ms);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);