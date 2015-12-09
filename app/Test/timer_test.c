/*
 * timer_test.c
 *
 *  Created on: Sep 8, 2015
 *      Author: z0037ysj
 */

#include "timer_test.h"
#include "timer.h"
#include "lcd.h"

static struct
{
    Word cntr;
    Bool light;
} test_wait = 
{ 
    0, 
    FALSE 
};

void test_timer_wait_init()
{    
    timersInit();
    displayInit();
}

void test_timer_wait_run(const Word cDelayToggle_x100ms)
{    
    waitX100us(1000);//100ms
    test_wait.cntr++;
    
    if (cDelayToggle_x100ms <= test_wait.cntr) 
    {
        test_wait.light = !test_wait.light;
        displayBackLightOn(test_wait.light);
        test_wait.cntr = 0;
    }
}

static Bool bLongTimeInsteadOfShort = FALSE;

void test_timer_startStop_init()
{
    displayOnOffControl_t control;
    
    bLongTimeInsteadOfShort = FALSE;
    displayClear();
    control.bBlinkingCursor = FALSE;
    control.bCursorOn = FALSE;
    control.bDisplayOn = TRUE;
    displayOnOffControl(control);
    
    displayWrite("<timer test>");
}

void test_timer_startStop_run()
{
    const int c3blinks = 6;
    static Bool bBacklighOn = TRUE;
    static int cntr = c3blinks;
    
    if (bLongTimeInsteadOfShort) 
    {
        if (timerElapsedSec())
        {
            cntr--;
            if (0 == cntr)
            {
                cntr = c3blinks;
                bLongTimeInsteadOfShort = (bLongTimeInsteadOfShort ? FALSE : TRUE);
            }
            else
            {                
                timerRestartSec(2);// 2 sec
                bBacklighOn = (bBacklighOn ? FALSE : TRUE);
                displayBackLightOn(bBacklighOn);
            }
        }
    }
    else 
    {        
        if (timerElapsedMiliSec())
        {
            cntr--;
            if (0 == cntr)
            {
                cntr = c3blinks;
                bLongTimeInsteadOfShort = (bLongTimeInsteadOfShort ? FALSE : TRUE);
            }
            else
            {                
                timerRestartMiliSec(500);// 500 ms
                bBacklighOn = (bBacklighOn ? FALSE : TRUE);
                displayBackLightOn(bBacklighOn);
            }
        }
    }
}
