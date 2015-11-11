/*
 * display.h
 *
 *  Created on: Aug 6, 2015
 *      Author: z0037ysj
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

typedef struct
{
    Bool    bDisplayOn;
    Bool    bCursorOn;
    Bool    bBlinkingCursor;
}displayOnOffControl_t;

typedef struct
{
    Bool bShiftScreenInsteadOfCursor;
    Bool bShiftRightInsteadOfLeft;
}displayMovingDirection_t;

static const Byte cDisplayAddressMax = 80;
static const Word cDisplayMaxContrast = 100;
static const Byte cDisplayNumOfChars = 16;
static const char cDisplayDegreeSymbol[2] = {223,0};// degree symbol = °

void displayInit();
void displayBackLightOn(const Bool bBackLightOn);
void displayClear();
void displayReturnHome();
void displayOnOffControl(const displayOnOffControl_t cControl);
void displayOrCursorShift(const displayMovingDirection_t cSetting);
void displayEntryModeSet(const displayMovingDirection_t cSetting);
void displayMoveCursor(const Byte cAddress);
void displayWrite(const char* pString);
void displaySetContrast(const Word cContrast);
Word displayGetContrast();

#ifdef __cplusplus
}
#endif
#endif /* DISPLAY_H_ */
