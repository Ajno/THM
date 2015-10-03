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
static Byte toggleBacklight = 5;

void controller()
{
	if (0 < toggleBacklight)
	{
		if (timerElapsed())
		{
			bBacklight = bBacklight ? FALSE : TRUE;
			displayBackLightOn(bBacklight);
			toggleBacklight--;
			if (0 != toggleBacklight)
			{
				timerRestart(500); // 500 ms
			}
		}
	}
}

void baseInitApp()
{
	bBacklight = FALSE;
	toggleBacklight = 5;
	baseInstallApp(&controller);
}
