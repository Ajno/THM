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
static lcdMovingDirection_t lcdDirection;
static Word contrast = cDefaultContrast;
static Byte shifts = 0;
static Byte animationState = 0;

void displayContrastSet()
{
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite("    ");
    lcdMoveCursor(cContrastPositionOnScreen);
    lcdWrite(thmLibItoa(contrast));
    lcdWrite("%");
}

void displayTemperatureSet(const sWord cTempInTenthsOfDeg)
{
    sWord temperature = cTempInTenthsOfDeg;

    // get cursor    
    lcdMoveCursor(cTemperaturePositionOnSreen);
    lcdWrite("      ");
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

void displayHumiditySet(const Word cHumidityInPercents)
{
    // get cursor
    lcdMoveCursor(cHumidityPositionOnSreen);
    lcdWrite("       ");
    lcdMoveCursor(cHumidityPositionOnSreen);
    lcdWrite(thmLibItoa(cHumidityInPercents));
    lcdWrite("%");
    // set cursor back
    lcdMoveCursor(cContrastPositionOnScreen);
}

void displayDoAnimation()
{
    lcdMoveCursor(cAnimationPositionOnSreen);
    switch (animationState)
    {
    case 0:
        lcdWrite("oooo (*_*) oooo");
        animationState = 1;
        break;
    case 1:
        lcdWrite("oooo (-_-) oooo");
        animationState = 2;
        break;
    case 2:
        lcdWrite("oOoo (-_-) ooOo");
        animationState = 3;
        break;
    case 3:
        lcdWrite("oOoo (-_-) ooOo");
        animationState = 4;
        break;
    case 4:
        lcdWrite("---> (-_-) ooOo");
        animationState = 5;
        break;
    case 5:
        lcdWrite("---->(o.o) ooOo");
        animationState = 6;
        break;
    case 6:
        lcdWrite("------->X) ooOo");
        animationState = 7;
        break;
    case 7:
        lcdWrite(">>-------> ooOo");
        animationState = 8;
        break;
    case 8:
        lcdWrite("  >>>------->Oo");
        animationState = 9;
        break;
    case 9:
        lcdWrite("    >>>------->");
        animationState = 10;
        break;
    case 10:
        lcdWrite("    >>>------->");
        animationState = 0;
        break;
    default:
        break;
    }
    // set cursor back
    lcdMoveCursor(cContrastPositionOnScreen);
}

void displayCursorTurnOn()
{
    lcdOnOff.bBlinkingCursor = TRUE;
    lcdOnOff.bCursorOn = TRUE;
    lcdOnOffControl(lcdOnOff);
    lcdMoveCursor(cContrastPositionOnScreen);
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
    lcdWrite("Teplota:        Kontrast:               Vlhkost:     ");
}

Bool displayIsOn()
{
    return lcdOnOff.bLcdOn;
}

Bool displayIsNotSliding()
{
    return (cLcdNumOfChars <= shifts);
}

void displaySlideRight()
{
    if (cLcdNumOfChars <= shifts)
    {
        shifts = 0;
    }
    lcdDirection.bShiftRightInsteadOfLeft = FALSE;
    lcdScreenOrCursorShift(lcdDirection);
    shifts++;
}

void displaySlideLeft()
{
    if (cLcdNumOfChars <= shifts)
    {
        shifts = 0;
    }
    lcdDirection.bShiftRightInsteadOfLeft = TRUE;
    lcdScreenOrCursorShift(lcdDirection);
    shifts++;
}

void displayContrastIncrement()
{
    contrast = lcdGetContrast();
    if (cLcdMaxContrast > contrast)
    {
        contrast += cContrastIncrement;
    }
    lcdSetContrast(contrast);
    displayContrastSet();
}

void displayContrastDecrement()
{
    contrast = lcdGetContrast();
    if (0 < contrast)
    {
        contrast -= cContrastIncrement;
    }
    lcdSetContrast(contrast);
    displayContrastSet();
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

void displayLowBatteryWarning()
{
    lcdReturnHome();
    lcdWrite("Low Battery!                            Low Battery!");
    // set cursor back
    lcdMoveCursor(cContrastPositionOnScreen);
}

void displayInit()
{
    bBacklightOn = FALSE;
    lcdOnOff.bLcdOn = FALSE;
    lcdOnOff.bBlinkingCursor = FALSE;
    lcdOnOff.bCursorOn = FALSE;
    lcdDirection.bShiftRightInsteadOfLeft = TRUE;
    lcdDirection.bShiftScreenInsteadOfCursor = TRUE;
    shifts = 0;
    contrast = cDefaultContrast;
    lcdSetContrast(contrast);
    animationState = 0;
}
