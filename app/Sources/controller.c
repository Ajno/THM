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
static Byte cntrBacklightToggle; // todo figure out why can't be initialized here with cToggleBacklight
static displayOnOffControl_t displayCntrl;

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
            displayWrite("Teplota neznama Kontrast: --            Vlhkost neznama  Jazyk: SK");
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
    }
}

void baseInitApp()
{
    bBacklight = FALSE;
    cntrBacklightToggle = cToggleBacklight;
    displayCntrl.bDisplayOn = FALSE;
    displayCntrl.bCursorOn = FALSE;
    displayCntrl.bBlinkingCursorOn = FALSE;
    baseInstallApp(&controller);
}
