/*
 * app.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include <stdio.h>

#include "base.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "pwr_mgmt.h"
#include "controller.h"

static const Word cAwakeTimeSec = 10;
static const Word cScreenShiftTimeMiliSec = 50;
static const Word cMenuActivationTimeMiliSec = 2000;
static const Word cContrastIncrement = 5;

static Bool                     bBacklightOn = FALSE;
static Byte                     cntrBacklightToggle = cNumOfBacklightToggle;
static displayOnOffControl_t    displayOnOff;
static displayMovingDirection_t displayDirection;
static Byte                     screenShifts = 0;
static Word                     contrast = 75;
static menuState_t              menu = cMenuState_display1;

static void screenShift()
{
    displayOrCursorShift(displayDirection);
    screenShifts++;
    timerRestartMiliSec(cScreenShiftTimeMiliSec);
}

void backlightOn()
{
    if (!bBacklightOn)
    {
        bBacklightOn = TRUE;
        displayBackLightOn(bBacklightOn);
    }
}

void contrastChange(const sWord cInc)
{
    char buffer[10];

    displayMoveCursor(cContrastPositionOnScreen);
    contrast = displayGetContrast();
    if ((cDisplayMaxContrast > contrast) && (0 < contrast))
    {
        contrast += cInc;
    }
    displaySetContrast(contrast);
    sprintf(buffer,"%d",contrast);
    displayWrite(buffer);
    displayWrite("%");
}

void cursorOn(const Bool cCursorOn)
{
    displayOnOff.bBlinkingCursor = cCursorOn;
    displayOnOff.bCursorOn = cCursorOn;
    displayOnOffControl(displayOnOff);
    menu = cMenuState_waitBeforeSelect2;
}

void onElapsedShortTimer()
{
    switch (menu)
    {
        case cMenuState_goto1:
            if (cDisplayNumOfChars <= screenShifts)
            {
                screenShifts = 0;
                menu = cMenuState_display1;
            }
            else
            {
                displayDirection.bShiftRightInsteadOfLeft = TRUE;
                screenShift();
            }
            break;
        case cMenuState_goto2:
            if (cDisplayNumOfChars <= screenShifts)
            {
                screenShifts = 0;
                menu = cMenuState_display2;
            }
            else
            {
                displayDirection.bShiftRightInsteadOfLeft = FALSE;
                screenShift();
            }
            break;
        case cMenuState_upperPressedInDisplay2:
            cursorOn(TRUE);
            displayMoveCursor(cContrastPositionOnScreen);
            break;
        case cMenuState_upperPressedInSelect2:
            cursorOn(FALSE);
            break;
        default:
            break;
    }
}

void onElapsedLongTimer()
{
    if (bBacklightOn)
    {
        bBacklightOn = FALSE;
        displayBackLightOn(bBacklightOn);
        timerRestartSec(cAwakeTimeSec);
    }
    else
    {
        displayOnOff.bDisplayOn = FALSE;
        displayOnOffControl(displayOnOff);
        pwrMgmtGoToSleep(TRUE);
    }
}

void processUpperButton()
{
    if (buttonIsPressed(cButton_Upper))
    {
        backlightOn();
        timerRestartSec(cAwakeTimeSec);

        switch (menu)
        {
            case cMenuState_display2:
                timerRestartMiliSec(cMenuActivationTimeMiliSec);
                menu = cMenuState_upperPressedInDisplay2;
                break;
            case cMenuState_select2:
                timerRestartMiliSec(cMenuActivationTimeMiliSec);
                menu = cMenuState_upperPressedInSelect2;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (menu)
        {
            case cMenuState_upperPressedInDisplay2:
                menu = cMenuState_display2;
                break;
            case cMenuState_waitBeforeSelect2:
                menu = cMenuState_select2;
                break;
            case cMenuState_upperPressedInSelect2:
                contrastChange(cContrastIncrement);
                menu = cMenuState_select2;
                break;
            case cMenuState_waitBeforeDisplay2:
                menu = cMenuState_display2;
                break;
            default:
                break;
        }
    }
}

void processLowerButton()
{
    if (buttonIsPressed(cButton_Lower))
    {
        backlightOn();
        timerRestartSec(cAwakeTimeSec);

        switch (menu)
        {
            case cMenuState_display1:
                displayDirection.bShiftRightInsteadOfLeft = FALSE;
                screenShift();
                menu = cMenuState_goto2;
                break;
            case cMenuState_display2:
                displayDirection.bShiftRightInsteadOfLeft = TRUE;
                screenShift();
                menu = cMenuState_goto1;
                break;
            case cMenuState_select2:
                menu = cMenuState_pressLowerWhenSelect2;
                break;
            default:
                break;
        }
    }
    else
    {
        if(cMenuState_pressLowerWhenSelect2 == menu)
        {
            contrastChange(-cContrastIncrement);
            menu = cMenuState_select2;
        }
    }
}

void controller()
{
    if (0 < cntrBacklightToggle)
    {
        if (timerElapsedMiliSec())
        {
            bBacklightOn = bBacklightOn ? FALSE : TRUE;
            displayBackLightOn(bBacklightOn);
            cntrBacklightToggle--;
            if (0 != cntrBacklightToggle)
            {
                timerRestartMiliSec(250); // 250 ms
            }
        }
    }
    else
    {
        if (!displayOnOff.bDisplayOn)
        {
            displayClear();
            displayOnOff.bDisplayOn = TRUE;
            displayOnOffControl(displayOnOff);
            backlightOn();
            displayWrite("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK");
            timerRestartSec(cAwakeTimeSec);
            menu = cMenuState_display1;
        }
        else
        {
            processLowerButton();
            processUpperButton();
        }

        if (timerElapsedSec())
        {
            onElapsedLongTimer();
        }

        if (timerElapsedMiliSec())
        {
            onElapsedShortTimer();
        }
    }
}

void baseInitApp()
{
    bBacklightOn = FALSE;
    cntrBacklightToggle = cNumOfBacklightToggle;
    displayOnOff.bDisplayOn = FALSE;
    displayOnOff.bCursorOn = FALSE;
    displayOnOff.bBlinkingCursor = FALSE;
    displayDirection.bShiftRightInsteadOfLeft = TRUE;
    displayDirection.bShiftScreenInsteadOfCursor = TRUE;
    menu = cMenuState_display1;
    screenShifts = 0;
    contrast = 75;
    displaySetContrast(contrast);

    baseInstallApp(&controller);
}
