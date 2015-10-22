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

static Bool bBacklight = FALSE;
static Byte cntrBacklightToggle = cNumOfBacklightToggle;
static displayOnOffControl_t displayCntrl;
static buttonState_t buttonState[2] =
{ cButtonState_Released, cButtonState_Released };
static screenState_t screenState = cScreenState_menu1;
static Byte screenPosition = 0;

static void shiftScreen(const Bool cDirection)
{
    displayMovingDirection_t direction;
    direction.bShiftRightInsteadOfLeft = cDirection;
    direction.bShiftScreenInsteadOfCursor = TRUE;
    displayOrCursorShift(direction);
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
            if (!bBacklight)
            {
                bBacklight = TRUE;
                displayBackLightOn(bBacklight);
            }
            displayWrite(
                    "Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK");
            timerRestartSec(5); // 5 sec
        }

        if (timerElapsedSec())
        {
            if (bBacklight)
            {
                bBacklight = FALSE;
                displayBackLightOn(bBacklight);
                timerRestartSec(5); // 5 sec
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
            if (cScreenState_gotoMenu2 == screenState)
            {
                shiftScreen(cToLeft);
                if ((cDisplayNumOfChars - 1) <= screenPosition)
                {
                    screenPosition = 0;
                    screenState = cScreenState_menu2;
                }
                else
                {
                    screenPosition++;
                    timerRestartMiliSec(250);
                }
            }
            else if (cScreenState_gotoMenu1 == screenState)
            {
                shiftScreen(cToRight);
                if ((cDisplayNumOfChars - 1) <= screenPosition)
                {
                    screenPosition = 0;
                    screenState = cScreenState_menu1;
                }
                else
                {
                    screenPosition++;
                    timerRestartMiliSec(250);
                }
            }
        }

        buttonStateDetection(cButton_Lower, &buttonState[cButton_Lower]);
        if (cButtonState_JustPressed == buttonState[cButton_Lower])
        {
            if (!bBacklight)
            {
                bBacklight = TRUE;
                displayBackLightOn(bBacklight);
                timerRestartSec(5); // 5 sec
            }

            if (cScreenState_menu1 == screenState)
            {
                shiftScreen(cToLeft);
                screenPosition++;
                timerRestartMiliSec(250);
                screenState = cScreenState_gotoMenu2;
            }
            else if (cScreenState_menu2 == screenState)
            {
                shiftScreen(cToRight);
                screenPosition++;
                timerRestartMiliSec(250);
                screenState = cScreenState_gotoMenu1;
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
    displayCntrl.bBlinkingCursorOn = FALSE;
    buttonState[cButton_Lower] = cButtonState_Released;
    buttonState[cButton_Upper] = cButtonState_Released;
    screenState = cScreenState_menu1;
    screenPosition = 0;

    baseInstallApp(&controller);
}
