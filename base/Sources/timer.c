/*
 * timer.c
 *
 *  Created on: Aug 24, 2015
 *      Author: z0037ysj
 */

#include "pwm.h"
#include "timer.h"

static Bool bBlockPwmTimerReset;

typedef struct 
{
    Word overflowCounter;
    Word remaining;
}timer_t;

static const Word cPwmModulo_200us = 800;
static const Word c100us = 400;
static const Word cNumOfOvrflwsIn1ms = 5;
static const Word cNumOfOvrflwsIn1sec = 5000;
static timer_t timerMiliSec;
static timer_t timerSec;

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
	Word t1 = 0;
	Word t2 = 0;
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

void timerRestartMiliSec(const Word cTimeout_ms)
{
    if (!bBlockPwmTimerReset)
    {
        // disable interrupt
        pwmSetOverflowInterruptEnable(FALSE);
        timerMiliSec.remaining = cTimeout_ms; 
        /*
         *  timer won't be possible to reset 
         *  again until next overflow in order to 
         *  ensure that no more than one PWM period
         *  can be skipped
         */
        bBlockPwmTimerReset = TRUE;        
        // reset PWM timer
        pwmClearTimer();
        // enable interrupt
        pwmSetOverflowInterruptEnable(TRUE);
    }
}

void timerRestartSec(const Word cTimeout_sec)
{
    timerSec.remaining = cTimeout_sec; 
}

Bool timerElapsedMiliSec()
{
    Bool ret = (timerMiliSec.remaining == 0);
    return ret;
}

Bool timerElapsedSec()
{
    Bool ret = (timerSec.remaining == 0);
    return ret;
}

static void processTimer(timer_t* pTimer, const Word cNumOfOvflws)
{
    if (0 != pTimer->remaining) 
    {
        if (0 != pTimer->overflowCounter) 
        {
            // count num of overflows, protect against wrong init
            pTimer->overflowCounter--;
        }
        
        if (0 == pTimer->overflowCounter) 
        {            
            pTimer->remaining--;
            pTimer->overflowCounter = cNumOfOvflws;
            
        }
    }
}

void timerIsrCallback()
{
    // timer can be reset again
    bBlockPwmTimerReset = FALSE;
    
    // stop-watch timer in miliseconds
    processTimer(&timerMiliSec, cNumOfOvrflwsIn1ms);
    
    // stop-watch timer in seconds
    processTimer(&timerSec, cNumOfOvrflwsIn1sec);
}

void timersInit()
{
	pwmTimerConfig_t cfg;
	
	bBlockPwmTimerReset = FALSE;
	timerMiliSec.remaining = 0;
	timerMiliSec.overflowCounter = cNumOfOvrflwsIn1ms; // 200us*5=1ms
    timerSec.remaining = 0;
    timerSec.overflowCounter = cNumOfOvrflwsIn1sec; // 200us*5000 = 1s
	
	pwmWriteModulo(cPwmModulo_200us);// 5kHz period
	
	cfg.clock = cPwmClk_bus;
	cfg.prescaler = cPwmPrsclr_1;// 4MHz clock
	cfg.bOverflowInterruptEnable = TRUE;
	pwmConfigureTimer(cfg);
	
	pwmInstallTimerIsrCallback(&timerIsrCallback);
}
