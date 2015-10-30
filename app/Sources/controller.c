/*
 * app.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "base.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "pwr_mgmt.h"
#include "controller.h"

static const Bool cToRight = TRUE;
static const Bool cToLeft = FALSE;
static const Word cAwakeTimeSec = 10;
static const Word cScreenShiftTimeMiliSec = 50;
static const Word cMenuActivationTimeMiliSec = 2000;

static Bool bBacklight = FALSE;
static Byte cntrBacklightToggle = cNumOfBacklightToggle;
static displayOnOffControl_t displayCntrl;
static menuState_t menu = cMenuState_show1;
static Byte screenShifts = 0;
static Byte contrast = 75;

static void screenShift(const Bool cDirection)
{
    displayMovingDirection_t direction;
    direction.bShiftRightInsteadOfLeft = cDirection;
    direction.bShiftScreenInsteadOfCursor = TRUE;
    displayOrCursorShift(direction);
    screenShifts++;
    timerRestartMiliSec(cScreenShiftTimeMiliSec);
}

void backlightTurnOn()
{
    if (!bBacklight)
    {
        bBacklight = TRUE;
        displayBackLightOn(bBacklight);
    }
}

void controller()
{
    if (0 < cntrBacklightToggle)
    {
        if (timerElapsedMiliSec())
        {
            bBacklight = bBacklight ? FALSE : TRUE;
            displayBackLightOn(bBacklight);
            cntrBacklightToggle--;
            if (0 != cntrBacklightToggle)
            {
                timerRestartMiliSec(250); // 250 ms
            }
        }
    }
    else
    {
        if (!displayCntrl.bDisplayOn)
        {
            displayClear();
            displayCntrl.bDisplayOn = TRUE;
            displayOnOffControl(displayCntrl);
            menu = cMenuState_show1;
            backlightTurnOn();
            displayWrite("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK");
            timerRestartSec(cAwakeTimeSec);
        }
        else
        {
            if (buttonIsPressed(cButton_Lower))
            {
                backlightTurnOn();
                timerRestartSec(cAwakeTimeSec);

                if (cMenuState_show1 == menu)
                {
                    screenShift(cToLeft);
                    menu = cMenuState_goto2;
                }
                else if (cMenuState_show2 == menu)
                {
                    screenShift(cToRight);
                    menu = cMenuState_goto1;
                }
            }

            if (buttonIsPressed(cButton_Upper))
            {
                backlightTurnOn();
                timerRestartSec(cAwakeTimeSec);

                if (cMenuState_show2 == menu)
                {
                    timerRestartMiliSec(cMenuActivationTimeMiliSec);
                    menu = cMenuState_selecting2;
                }
                else if (cMenuState_modify2 == menu)
                {
                    timerRestartMiliSec(cMenuActivationTimeMiliSec);
                    menu = cMenuState_unselecting2;
                }
            }
            else
            {
                // stop selecting the menu 2
                if (cMenuState_selecting2 == menu)
                {
                    menu = cMenuState_show2;
                }
                else if(cMenuState_selected2 == menu)
                {
                    menu = cMenuState_modify2;
                }
                else if(cMenuState_unselecting2 == menu)
                {
                    menu = cMenuState_modify2;
                    // todo add contrast also in real world :-)
                    displayMoveCursor(cCursorPosEndOfLine1Menu2 - 3);
                    displayWrite("80%");
                }
                else if(cMenuState_unselected2 == menu)
                {
                    menu = cMenuState_show2;
                }
            }
        }

        if (timerElapsedSec())
        {
            if (bBacklight)
            {
                bBacklight = FALSE;
                displayBackLightOn(bBacklight);
                timerRestartSec(cAwakeTimeSec);
            }
            else
            {
                displayCntrl.bDisplayOn = FALSE;
                displayOnOffControl(displayCntrl);
                pwrMgmtGoToSleep(TRUE);
            }
        }

        if (timerElapsedMiliSec())
        {
            if (cMenuState_goto2 == menu)
            {
                if (cDisplayNumOfChars <= screenShifts)
                {
                    screenShifts = 0;
                    menu = cMenuState_show2;
                }
                else
                {
                    screenShift(cToLeft);
                }
            }
            else if (cMenuState_goto1 == menu)
            {
                if (cDisplayNumOfChars <= screenShifts)
                {
                    screenShifts = 0;
                    menu = cMenuState_show1;
                }
                else
                {
                    screenShift(cToRight);
                }
            }
            else if (cMenuState_selecting2 == menu)
            {
                displayOnOffControl_t onOffControl;
                menu = cMenuState_selected2;
                onOffControl.bBlinkingCursor = TRUE;
                onOffControl.bDisplayOn = TRUE;
                onOffControl.bCursorOn = TRUE;
                displayOnOffControl(onOffControl);
                displayMoveCursor(cCursorPosEndOfLine1Menu2);
            }
            else if (cMenuState_unselecting2 == menu)
            {
                displayOnOffControl_t onOffControl;
                menu = cMenuState_unselected2;
                onOffControl.bBlinkingCursor = FALSE;
                onOffControl.bDisplayOn = TRUE;
                onOffControl.bCursorOn = FALSE;
                displayOnOffControl(onOffControl);
            }
        }
    }
}

void baseInitApp()
{
    bBacklight = FALSE;
    cntrBacklightToggle = cNumOfBacklightToggle;
    displayCntrl.bDisplayOn = FALSE;
    displayCntrl.bCursorOn = FALSE;
    displayCntrl.bBlinkingCursor = FALSE;
    menu = cMenuState_show1;
    screenShifts = 0;
    contrast = 75;

    baseInstallApp(&controller);
}
