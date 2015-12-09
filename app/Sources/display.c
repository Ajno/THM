/*
 * display.c
 *
 *  Created on: Dec 9, 2015
 *      Author: z0037ysj
 */

#include "display.h"
#include "lcd.h"

static Bool bBacklightOn = FALSE;

void displayBacklightTurnOn()
{
    if (!bBacklightOn)
    {
        bBacklightOn = TRUE;
        lcdBackLightOn(bBacklightOn);
    }
}

Bool displayBacklightIsOn()
{
    return bBacklightOn;
}

void displayBacklightTurnOff()
{
    bBacklightOn = FALSE;
    lcdBackLightOn(bBacklightOn);
}

void displayBacklightToggle()
{
    bBacklightOn = bBacklightOn ? FALSE : TRUE;
    lcdBackLightOn(bBacklightOn);
}

void displayInit()
{
    bBacklightOn = FALSE;
}
