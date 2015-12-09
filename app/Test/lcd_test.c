/*
 * display_test.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: z0037ysj
 */

#include "lcd_test.h"
#include "timer.h"
#include "buttons.h"

void test_display_shiftAndBackLight_init()
{
    lcdOnOffControl_t onOffSetting;

    timersInit();
    lcdInit();
    buttonsInit();

    lcdBackLightOn(TRUE);

    // Display on
    onOffSetting.bLcdOn = TRUE;
    onOffSetting.bCursorOn = TRUE;
    onOffSetting.bBlinkingCursor = TRUE;
    lcdOnOffControl(onOffSetting);

    //Display clear
    lcdClear();

    lcdWrite("vrchnym tlacitkom zmen smer");
    lcdMoveCursor(cLcdAddressMax/2);
    lcdWrite("spodnym tlacitkom vypni/zapni LED");
}

void test_display_shiftAndBackLight_run()
{
    static lcdMovingDirection_t dir = {TRUE, TRUE};
    static Word cntr = 0;
    static Bool bBacklightOn = TRUE;
    static Bool stateLowerCurrent = FALSE;
    static Bool stateUpperCurrent = FALSE;
    static Bool stateLowerPrevious = FALSE;
    static Bool stateUpperPrevious = FALSE;
    
    waitX100us(500); // wait 50 ms
    cntr++;
    
    stateUpperCurrent = buttonIsPressed(cButton_Upper);
    if ((!stateUpperPrevious) && (stateUpperCurrent))
    {
        dir.bShiftRightInsteadOfLeft = 
                (dir.bShiftRightInsteadOfLeft ? FALSE : TRUE );
    }
    
    stateLowerCurrent = buttonIsPressed(cButton_Lower);
    if ((!stateLowerPrevious) && (stateLowerCurrent))
    {
        bBacklightOn = (bBacklightOn ? FALSE : TRUE );
        lcdBackLightOn(bBacklightOn);
    }
    
    if (cntr >= 5)
    {        
        dir.bShiftScreenInsteadOfCursor = TRUE;   
        lcdScreenOrCursorShift(dir);
        cntr = 0;
    }
    
    stateUpperPrevious = stateUpperCurrent;
    stateLowerPrevious = stateLowerCurrent;
}

void test_display_contrast_init()
{
    lcdOnOffControl_t onOffSetting;

    timersInit();
    buttonsInit();

    lcdBackLightOn(TRUE);

    // Display on
    onOffSetting.bLcdOn = TRUE;
    onOffSetting.bCursorOn = TRUE;
    onOffSetting.bBlinkingCursor = TRUE;
    lcdOnOffControl(onOffSetting);

    //Display clear
    lcdClear();

    lcdWrite("contrast:");
    lcdMoveCursor(cLcdAddressMax/2);
    lcdWrite("red(+), black(-)");
}

void test_display_contrast_run()
{
    static Word contrastOut = 50;
    static Word contrastIn = 0;
    static Bool stateLowerCurrent = FALSE;
    static Bool stateUpperCurrent = FALSE;
    static Bool stateLowerPrevious = FALSE;
    static Bool stateUpperPrevious = FALSE;
    
    stateUpperCurrent = buttonIsPressed(cButton_Upper);
    if ((!stateUpperPrevious) && (stateUpperCurrent))
    {
        if (100 != contrastOut) 
        {
            contrastOut = contrastOut + 5;            
            lcdSetContrast(contrastOut);
            contrastIn = lcdGetContrast();
        }
    }
    
    stateLowerCurrent = buttonIsPressed(cButton_Lower);
    if ((!stateLowerPrevious) && (stateLowerCurrent))
    {
        if (0 != contrastOut) 
        {
            contrastOut = contrastOut - 5;   
            lcdSetContrast(contrastOut);
            contrastIn = lcdGetContrast();
        }
    }
    
    stateUpperPrevious = stateUpperCurrent;
    stateLowerPrevious = stateLowerCurrent;
}
