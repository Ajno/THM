/*
 * display.c
 *
 *  Created on: Dec 9, 2015
 *      Author: z0037ysj
 */

#include "display.h"
#include "lcd.h"
#include "thm_lib.h"

static Bool bBacklightOn = FALSE;

void displayUpdateContrast(const Word cContrast)
{
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite("    ");
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite(thmLibItoa(cContrast));
    lcdWrite("%");
}

void displayUpdateTemperature(const sWord cTemperature)
{
    sWord temperature = cTemperature;
    
    // get cursor    
    lcdMoveCursor(cTemperaturePositionOnSreen);
    lcdWrite("    ");
    lcdMoveCursor(cTemperaturePositionOnSreen);
    lcdWrite(thmLibItoa(temperature / 10));
    lcdWrite(",");
    if (0 > temperature)
    {
        temperature = -temperature;
    }
    lcdWrite(thmLibItoa(temperature % 10));
    lcdWrite(&cLcdDegreeSymbol);
    lcdWrite("C");
    // set cursor back
    lcdMoveCursor(cContrastPositionOnScreen);
}

void displayUpdateHumidity(const Word cHumidity)
{
    // get cursor
    lcdMoveCursor(cHumidityPositionOnSreen);
    lcdWrite("      ");
    lcdMoveCursor(cHumidityPositionOnSreen);
    lcdWrite(thmLibItoa(cHumidity));
    lcdWrite("%");
    // set cursor back
    lcdMoveCursor(cContrastPositionOnScreen);
}

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
