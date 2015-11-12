/*
 * app.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "thm_lib.h"

#include "base.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "pwr_mgmt.h"
#include "temperature.h"
#include "controller.h"

static const Word cAwakeTimeSec = 10;
static const Word cScreenShiftTimeMiliSec = 50;
static const Word cMenuActivationTimeMiliSec = 2000;
static const Word cTemperatureSamplingMiliSecX100 = 3; // 300 ms
static const Word cContrastIncrement = 5;

static Bool                     bBacklightOn = FALSE;
static Byte                     cntrBacklightToggle = cNumOfBacklightToggle;
static displayOnOffControl_t    displayOnOff;
static displayMovingDirection_t displayDirection;
static Byte                     screenShifts = 0;
static Word                     contrast = 75;
static menuState_t              menu = cMenuState_idle1;

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

void contrastUpdateOnScreen()
{
    displayMoveCursor(cContrastPositionOnScreen);
    displayWrite("    ");
    displayMoveCursor(cContrastPositionOnScreen);
    displayWrite(thmLibItoa(contrast));
    displayWrite("%");
}

void temperatureUpdateOnScreen()
{
    sWord temperatureRaw = 0;

    displayMoveCursor(cTemperaturePositionOnSreen);
    displayWrite("    ");
    displayMoveCursor(cTemperaturePositionOnSreen);
    temperatureRaw = temperatureRead();
    displayWrite(thmLibItoa(temperatureRaw / 10));
    displayWrite(",");
    displayWrite(thmLibItoa(temperatureRaw % 10));
    displayWrite(&cDisplayDegreeSymbol);
    displayWrite("C");
}

void contrastAdd(const Word cAdd)
{
    contrast = displayGetContrast();
    if (cDisplayMaxContrast > contrast)
    {
        contrast += cAdd;
    }
    displaySetContrast(contrast);
    contrastUpdateOnScreen();
}

void contrastDec(const Word cDecrease)
{
    contrast = displayGetContrast();
    if (0 < contrast)
    {
        contrast -= cDecrease;
    }
    displaySetContrast(contrast);
    contrastUpdateOnScreen();
}

void cursorOn(const Bool cCursorOn)
{
    displayOnOff.bBlinkingCursor = cCursorOn;
    displayOnOff.bCursorOn = cCursorOn;
    displayOnOffControl(displayOnOff);
}

void onElapsedVeryShortTimer()
{
    switch (menu)
    {
        case cMenuState_goto1:
            if (cDisplayNumOfChars <= screenShifts)
            {
                screenShifts = 0;
                menu = cMenuState_idle1;
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
                menu = cMenuState_idle2;
            }
            else
            {
                displayDirection.bShiftRightInsteadOfLeft = FALSE;
                screenShift();
            }
            break;
        case cMenuState_upperPressedInIdle2:
            cursorOn(TRUE);
            displayMoveCursor(cContrastPositionOnScreen);
            menu = cMenuState_waitToChangeContrast;
            break;
        case cMenuState_upperPressedInChangeContrast:
            cursorOn(FALSE);
            menu = cMenuState_waitToEnterIdle2;
            break;
        default:
            break;
    }
}

void onElapsedShortTimer()
{
    temperatureUpdateOnScreen();
    timerRestartMiliSecX100(cTemperatureSamplingMiliSecX100);
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
        menu = cMenuState_sleep;
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
            case cMenuState_idle2:
                timerRestartMiliSec(cMenuActivationTimeMiliSec);
                menu = cMenuState_upperPressedInIdle2;
                break;
            case cMenuState_changeContrast:
                timerRestartMiliSec(cMenuActivationTimeMiliSec);
                menu = cMenuState_upperPressedInChangeContrast;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (menu)
        {
            case cMenuState_upperPressedInIdle2:
                menu = cMenuState_idle2;
                break;
            case cMenuState_waitToChangeContrast:
                menu = cMenuState_changeContrast;
                break;
            case cMenuState_upperPressedInChangeContrast:
                contrastAdd(cContrastIncrement);
                menu = cMenuState_changeContrast;
                break;
            case cMenuState_waitToEnterIdle2:
                menu = cMenuState_idle2;
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
            case cMenuState_idle1:
                displayDirection.bShiftRightInsteadOfLeft = FALSE;
                screenShift();
                menu = cMenuState_goto2;
                break;
            case cMenuState_idle2:
                displayDirection.bShiftRightInsteadOfLeft = TRUE;
                screenShift();
                menu = cMenuState_goto1;
                break;
            case cMenuState_changeContrast:
                menu = cMenuState_lowerPressedInChangeContrast;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (menu)
        {
            case cMenuState_lowerPressedInChangeContrast:
                contrastDec(cContrastIncrement);
                menu = cMenuState_changeContrast;
                break;
            case cMenuState_sleep:
                menu = cMenuState_idle1;
                break;
            default:
                break;
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
            if (displayOnOff.bCursorOn)
            {
                displayOnOff.bCursorOn = FALSE;
                displayOnOff.bBlinkingCursor = FALSE;
            }
            displayOnOffControl(displayOnOff);
            backlightOn();
            displayWrite("Teplota:        Kontrast:               Vlhkost neznama Jazyk: SVK");
            temperatureUpdateOnScreen();
            contrastUpdateOnScreen();
            timerRestartSec(cAwakeTimeSec);
        }
        else
        {
            processLowerButton();
            processUpperButton();
        }

        if (timerElapsedMiliSec())
        {
            onElapsedVeryShortTimer();
        }

        if (timerElapsedMiliSecX100())
        {
            onElapsedShortTimer();
        }

        if (timerElapsedSec())
        {
            onElapsedLongTimer();
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
    menu = cMenuState_idle1;
    screenShifts = 0;
    contrast = 50;
    displaySetContrast(contrast);

    baseInstallApp(&controller);
}
