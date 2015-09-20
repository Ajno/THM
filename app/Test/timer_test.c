/*
 * timer_test.c
 *
 *  Created on: Sep 8, 2015
 *      Author: z0037ysj
 */

#include "timer_test.h"
#include "timer.h"
#include "display.h"

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
    timerInit();
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

void test_timer_startStop_init()
{
    displayOnOffControl_t control;
    
    timerInit();
    displayInit();
    displayClear();
    displayWaitTillNotBusy();
    control.bBlinkingCursorOn = FALSE;
    control.bCursorOn = FALSE;
    control.bDisplayOn = TRUE;
    displayOnOffControl(control);
    displayWaitTillNotBusy();
    
    displayWrite("<timer test>");
}

void test_timer_startStop_run()
{
    static Bool bBacklighOn = TRUE;
    
    if (timerElapsed())
    {
        timerRestart(1000);// 1 sec
        bBacklighOn = (bBacklighOn ? FALSE : TRUE);
        displayBackLightOn(bBacklighOn);
    }
}
