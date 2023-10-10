/*
 * File:   io.c
 * Author: szeyu
 *
 * Created on October 10, 2023, 9:04 AM
 */


#include "xc.h"

void IOinit()
{
    AD1PCFG = 0xFFFF;
}
void IOcheck();

