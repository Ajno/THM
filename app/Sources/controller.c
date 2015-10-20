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

static Bool bBacklight = FALSE;
static Byte cntrBacklightToggle = cToggleBacklight;
static displayOnOffControl_t displayCntrl;
static buttonState_t buttonState[2] = {cButtonState_Released, cButtonState_Released};
static Byte currentScreen = 1;

void controller()
{
    if (0 < cntrBacklightToggle)
    {
        if (timerElapsed())
        {
            bBacklight = bBacklight ? FALSE : TRUE;
            displayBackLightOn(bBacklight);
            cntrBacklightToggle--;
            if (0 != cntrBacklightToggle)
            {
                timerRestart(250); // 250 ms
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
            displayWrite("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK");
            timerRestart(5000); // 5 sec
        }

        if (timerElapsed())
        {
            if (bBacklight)
            {
                bBacklight = FALSE;
                displayBackLightOn(bBacklight);
                timerRestart(5000); // 5 sec
            }
            else
            {
                displayCntrl.bDisplayOn = FALSE;
                displayOnOffControl(displayCntrl);
                pwrMgmtGoToSleep(TRUE);
            }
        }

        buttonStateDetection(cButton_Lower, &buttonState[cButton_Lower]);
        if (cButtonState_JustPressed == buttonState[cButton_Lower])
        {
            if (!bBacklight)
            {
                bBacklight = TRUE;
                displayBackLightOn(bBacklight);
                timerRestart(5000); // 5 sec
            }

            if (1 == currentScreen)
            {
                Byte i;
                displayMovingDirection_t direction;
                direction.bShiftRightInsteadOfLeft = FALSE;
                direction.bShiftScreenInsteadOfCursor = TRUE;
                for (i = 0; i < 16; ++i)
                {
                    displayOrCursorShift(direction);
                }
                currentScreen = 2;
            }
            else if (2 == currentScreen)
            {
                Byte i;
                displayMovingDirection_t direction;
                direction.bShiftRightInsteadOfLeft = TRUE;
                direction.bShiftScreenInsteadOfCursor = TRUE;
                for (i = 0; i < 16; ++i)
                {
                    displayOrCursorShift(direction);
                }
                currentScreen = 1;
            }
        }
    }
}

void baseInitApp()
{
    bBacklight = FALSE;
    cntrBacklightToggle = cToggleBacklight;
    displayCntrl.bDisplayOn = FALSE;
    displayCntrl.bCursorOn = FALSE;
    displayCntrl.bBlinkingCursorOn = FALSE;
    buttonState[cButton_Lower] = cButtonState_Released;
    buttonState[cButton_Upper] = cButtonState_Released;
    currentScreen = 1;

    baseInstallApp(&controller);
}
