/*
 * display.h
 *
 *  Created on: Dec 9, 2015
 *      Author: z0037ysj
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

enum displayConst_t
{
    cTemperaturePositionOnSreen = 0x09,
    cContrastPositionOnScreen   = 0x1a,
    cHumidityPositionOnSreen    = 0x49,
    cAnimationPositionOnSreen   = 0x50,
    cDefaultContrast            = 40,
    cContrastIncrement          = 5,
    cTimeoutScreenShift         = 50 // 50 ms
};

void displayContrastSet();
void displayTemperatureSet(const sWord cTempInTenthsOfDeg);
void displayHumiditySet(const Word cHumidityInPercents);
void displayDoAnimation();
void displayCursorTurnOn();
void displayCursorTurnOff();
void displayMenuTemplate();
void displayTurnOff();
void displayTurnOn();
Bool displayIsOn();
Bool displayIsNotSliding();
void displaySlideRight();
void displaySlideLeft();
void displayContrastIncrement();
void displayContrastDecrement();
void displayBacklightTurnOn();
void displayBacklightTurnOff();
void displayBacklightToggle();
Bool displayBacklightIsOn();
void displayLowBatteryWarning();
void displayInit();

#ifdef __cplusplus
}
#endif
#endif /* DISPLAY_H_ */
