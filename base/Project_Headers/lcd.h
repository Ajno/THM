/*
 * display.h
 *
 *  Created on: Aug 6, 2015
 *      Author: z0037ysj
 */

#ifndef LCD_H_
#define LCD_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

typedef struct
{
    Bool    bLcdOn;
    Bool    bCursorOn;
    Bool    bBlinkingCursor;
}lcdOnOffControl_t;

typedef struct
{
    Bool bShiftScreenInsteadOfCursor;
    Bool bShiftRightInsteadOfLeft;
}lcdMovingDirection_t;

static const Byte cLcdAddressMax = 80;
static const Word cLcdMaxContrast = 100;
static const Byte cLcdNumOfChars = 16;
static const char cLcdDegreeSymbol[2] = {223,0};// degree symbol = °

void lcdInit();
void lcdBackLightOn(const Bool bBackLightOn);
void lcdClear();
void lcdReturnHome();
void lcdOnOffControl(const lcdOnOffControl_t cControl);
void lcdScreenOrCursorShift(const lcdMovingDirection_t cSetting);
void lcdEntryModeSet(const lcdMovingDirection_t cSetting);
void lcdMoveCursor(const Byte cAddress);
void lcdWrite(const char* pString);
void lcdSetContrast(const Word cContrast);
Word lcdGetContrast();
void lcdPrepareForSleep();
void lcdWakeUpCallback();

#ifdef __cplusplus
}
#endif
#endif /* DISPLAY_H_ */
