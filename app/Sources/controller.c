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
#include "lcd.h"
#include "display.h"
#include "pwr_mgmt.h"
#include "temperature.h"
#include "controller.h"

static const Word cAwakeTimeSec = 10;
static const Word cScreenShiftTimeMiliSec = 50;
static const Word cMenuActivationTimeMiliSec = 2000;
static const Word cTemperatureSamplingMiliSecX100 = 5; // 500 ms
static const Word cContrastIncrement = 5;

static Byte                 cntrBacklightToggle = cNumOfBacklightToggle;
static lcdOnOffControl_t    lcdOnOff;
static lcdMovingDirection_t lcdDirection;
static Byte                 screenShifts = 0;
static Word                 contrast = 75;
static menuState_t          menu = cMenuState_idle1;

static void screenShift()
{
    lcdScreenOrCursorShift(lcdDirection);
    screenShifts++;
    timerRestartMiliSec(cScreenShiftTimeMiliSec);
}

void contrastUpdateOnScreen()
{
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite("    ");
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite(thmLibItoa(contrast));
    lcdWrite("%");
}

void temperatureUpdateOnScreen()
{
    sWord temperatureRaw = 0;

    // get cursor    
    lcdMoveCursor(cTemperaturePositionOnSreen);
    lcdWrite("    ");
    lcdMoveCursor(cTemperaturePositionOnSreen);
    temperatureRaw = temperatureRead();
    lcdWrite(thmLibItoa(temperatureRaw / 10));
    lcdWrite(",");
    if (0 > temperatureRaw)
    {
        temperatureRaw = -temperatureRaw;
    }
    lcdWrite(thmLibItoa(temperatureRaw % 10));
    lcdWrite(&cLcdDegreeSymbol);
    lcdWrite("C");
    // set cursor back
    lcdMoveCursor(cContrastPositionOnScreen);
}

void contrastAdd(const Word cAdd)
{
    contrast = lcdGetContrast();
    if (cLcdMaxContrast > contrast)
    {
        contrast += cAdd;
    }
    lcdSetContrast(contrast);
    contrastUpdateOnScreen();
}

void contrastDec(const Word cDecrease)
{
    contrast = lcdGetContrast();
    if (0 < contrast)
    {
        contrast -= cDecrease;
    }
    lcdSetContrast(contrast);
    contrastUpdateOnScreen();
}

void cursorOn(const Bool cCursorOn)
{
    lcdOnOff.bBlinkingCursor = cCursorOn;
    lcdOnOff.bCursorOn = cCursorOn;
    lcdOnOffControl(lcdOnOff);
}

void onElapsedVeryShortTimer()
{
    switch (menu)
    {
        case cMenuState_goto1:
            if (cLcdNumOfChars <= screenShifts)
            {
                screenShifts = 0;
                menu = cMenuState_idle1;
            }
            else
            {
                lcdDirection.bShiftRightInsteadOfLeft = TRUE;
                screenShift();
            }
            break;
        case cMenuState_goto2:
            if (cLcdNumOfChars <= screenShifts)
            {
                screenShifts = 0;
                menu = cMenuState_idle2;
            }
            else
            {
                lcdDirection.bShiftRightInsteadOfLeft = FALSE;
                screenShift();
            }
            break;
        case cMenuState_upperPressedInIdle2:
            cursorOn(TRUE);
            lcdMoveCursor(cContrastPositionOnScreen);
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
    if (displayBacklightIsOn())
    {
        displayBacklightTurnOff();
        timerRestartSec(cAwakeTimeSec);
    }
    else
    {
        lcdOnOff.bLcdOn = FALSE;
        lcdOnOffControl(lcdOnOff);
        menu = cMenuState_sleep;
        lcdPrepareForSleep();
        pwrMgmtGoToSleep(TRUE);
    }
}

void processUpperButton()
{
    if (buttonIsPressed(cButton_Upper))
    {
        displayBacklightTurnOn();
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
        displayBacklightTurnOn();
        timerRestartSec(cAwakeTimeSec);

        switch (menu)
        {
            case cMenuState_idle1:
                lcdDirection.bShiftRightInsteadOfLeft = FALSE;
                screenShift();
                menu = cMenuState_goto2;
                break;
            case cMenuState_idle2:
                lcdDirection.bShiftRightInsteadOfLeft = TRUE;
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
            displayBacklightToggle();
            cntrBacklightToggle--;
            if (0 != cntrBacklightToggle)
            {
                timerRestartMiliSec(250); // 250 ms
            }
        }
    }
    else
    {
        if (!lcdOnOff.bLcdOn)
        {
            lcdClear();
            lcdOnOff.bLcdOn = TRUE;
            if (lcdOnOff.bCursorOn)
            {
                lcdOnOff.bCursorOn = FALSE;
                lcdOnOff.bBlinkingCursor = FALSE;
            }
            lcdOnOffControl(lcdOnOff);
            displayBacklightTurnOn();
            lcdWrite("Teplota:        Kontrast:               Vlhkost neznama Jazyk: SVK");
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
    cntrBacklightToggle = cNumOfBacklightToggle;
    lcdOnOff.bLcdOn = FALSE;
    lcdOnOff.bCursorOn = FALSE;
    lcdOnOff.bBlinkingCursor = FALSE;
    lcdDirection.bShiftRightInsteadOfLeft = TRUE;
    lcdDirection.bShiftScreenInsteadOfCursor = TRUE;
    menu = cMenuState_idle1;
    screenShifts = 0;
    contrast = 50;
    lcdSetContrast(contrast);

    displayInit();
    baseInstallApp(&controller);
}
