/*
 * oscillatortest.c
 *
 *  Created on: Nov 20, 2015
 *      Author: z0037ysj
 */

#include "oscillator_test.h"
#include "timer.h"

void test_oscillator_init()
{
    oscillatorInit();
}

void test_oscillator_run()
{
    static Word oscillator = 0;
    
    if (timerElapsedSec()) 
    {
        oscillator = oscillatorReadAndClear();
        timerRestartSec(50);
    }
}
