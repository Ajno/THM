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
static lcdOnOffControl_t lcdOnOff;
static Word contrast = cDefaultContrast;

void displayContrastSet()
{
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite("    ");
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite(thmLibItoa(contrast));
    lcdWrite("%");
}

void displayTemperatureSet(const sWord cTemperature)
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

void displayHumiditySet(const Word cHumidity)
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

void displayCursorTurnOn()
{
    lcdOnOff.bBlinkingCursor = TRUE;
    lcdOnOff.bCursorOn = TRUE;
    lcdOnOffControl(lcdOnOff);
}

void displayCursorTurnOff()
{
    lcdOnOff.bBlinkingCursor = FALSE;
    lcdOnOff.bCursorOn = FALSE;
    lcdOnOffControl(lcdOnOff);    
}

void displayTurnOff()
{
    lcdOnOff.bLcdOn = FALSE;
    lcdOnOffControl(lcdOnOff);
    lcdPrepareForSleep();
}

void displayTurnOn()
{
    lcdClear();
    lcdOnOff.bLcdOn = TRUE;
    if (lcdOnOff.bCursorOn)
    {
        lcdOnOff.bCursorOn = FALSE;
        lcdOnOff.bBlinkingCursor = FALSE;
    }
    lcdOnOffControl(lcdOnOff);    
}

Bool displayIsOn()
{
    return lcdOnOff.bLcdOn;
}

void displayContrastIncrement()
{
    contrast = lcdGetContrast();
    if (cLcdMaxContrast > contrast)
    {
        contrast += cContrastIncrement;
    }
    lcdSetContrast(contrast);
    displayContrastSet(contrast);
}

void displayContrastDecrement()
{
    contrast = lcdGetContrast();
    if (0 < contrast)
    {
        contrast -= cContrastIncrement;
    }
    lcdSetContrast(contrast);
    displayContrastSet(contrast);
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
    lcdOnOff.bLcdOn = FALSE;
    lcdOnOff.bBlinkingCursor = FALSE;
    lcdOnOff.bCursorOn = FALSE;
    contrast = cDefaultContrast;
    lcdSetContrast(contrast);
}
