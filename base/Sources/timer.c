/*
 * timer.c
 *
 *  Created on: Aug 24, 2015
 *      Author: z0037ysj
 */

#include "pwm.h"
#include "timer.h"

typedef struct 
{
    Bool bBlockReset;
    Word overflowCounter;
    Word remaining_ms;
}timer_t;

static const Word cPwmModulo_200us = 800;
static const Word c100us = 400;
static const Word cNumOfOvrflwsIn1ms = 5;
static timer_t timer;

void wait500ns()
{
    // delay two bus cycles = 500 ns
    asm
    {
        NOP
        NOP
    }
}

void waitX100us(const Word cDelay)
{
	volatile Word t1 = 0;
	volatile Word t2 = 0;
	Word counter = cDelay;
	
	t1 = pwmReadTimer();
	do
	{
		do
		{
			t2 = pwmReadTimer();
			if (t2 < t1)
			{
				t2 = cPwmModulo_200us - t1;
			}
		} while ((t2 - t1) < c100us);
		t1 = t2;
		counter--;
	} while (0 != counter);
}

void timerRestart(const Word cTimeout_ms)
{
    if (!timer.bBlockReset)
    {
        // disable interrupt
        pwmSetOverflowInterruptEnable(FALSE);
        timer.remaining_ms = cTimeout_ms; 
        // timer won't be able to reset again until next overflow
        timer.bBlockReset = TRUE;        
        // reset PWM timer
        pwmClearTimer();
        // enable interrupt
        pwmSetOverflowInterruptEnable(TRUE);
    }
}

Bool timerElapsed()
{
    Bool ret = (timer.remaining_ms == 0);
    return ret;
}

void timerIsrCallback()
{
    // timer can be reset again
    timer.bBlockReset = FALSE;
    
    if (0 != timer.remaining_ms) 
    {
        if (0 != timer.overflowCounter) 
        {
            // count num of overflows, protect against wrong init
            timer.overflowCounter--;
        }
        
        if (0 == timer.overflowCounter) 
        {            
            timer.remaining_ms--;
            timer.overflowCounter = cNumOfOvrflwsIn1ms;            
        }
    }
}

void timerInit()
{
	pwmTimerConfig_t cfg;
	
	timer.remaining_ms = 0;
	timer.bBlockReset = FALSE;
	timer.overflowCounter = cNumOfOvrflwsIn1ms; // 200us*5=1ms
	
	pwmWriteModulo(cPwmModulo_200us);// 5kHz period
	
	cfg.clock = cPwmClk_bus;
	cfg.prescaler = cPwmPrsclr_1;// 4MHz clock
	cfg.bOverflowInterruptEnable = FALSE;
	pwmConfigureTimer(cfg);
	
	pwmInstallTimerIsrCallback(&timerIsrCallback);
}
