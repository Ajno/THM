/*
 * lcd_mock.cpp
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#include <lcd_mock.h>
#include <lcd.h>

static bool bBacklightIsOn = false;
static bool bLcdOn = false;
static Word contrast = 0;
static LcdMock::screen_t screen = {"", 0};
static LcdMock::cursor_t cursor = {LcdMock::cCursor_off, 0};

extern "C" void lcdBackLightOn(const Bool bBackLightOn)
{
	bBacklightIsOn = bBackLightOn;
}

extern "C" void lcdOnOffControl(const lcdOnOffControl_t cControl)
{
	bLcdOn = cControl.bLcdOn;
	if (cControl.bBlinkingCursor && cControl.bCursorOn)
	{
		cursor.state = LcdMock::cCursor_blinking;
	}
	else if (cControl.bCursorOn)
	{
	    cursor.state = LcdMock::cCursor_on;
	}
	else
	{
	    cursor.state = LcdMock::cCursor_off;
	}
}

extern "C" void lcdClear()
{
    screen.text = "";
    screen.position = 0;
    cursor.position = 0;
}

// the real LCD addresses are hexadecimal
extern "C" void lcdWrite(const char* pString)
{
    string str(pString);

    if (64 <= cursor.position)
    {
        cursor.position -= 24;
    }

    if ((80-24) > cursor.position)
    {
        if (static_cast<unsigned int>(cursor.position) <= screen.text.length())
        {
            screen.text.replace(cursor.position, str.length(), str);
            cursor.position += str.length();
        }
    }
}

extern "C" void lcdScreenOrCursorShift(const lcdMovingDirection_t cSetting)
{
    if (cSetting.bShiftRightInsteadOfLeft)
    {
        if (cSetting.bShiftScreenInsteadOfCursor)
        {
            screen.position--;
        }
        else
        {

        }
    }
    else
    {
        if (cSetting.bShiftScreenInsteadOfCursor)
        {
            screen.position++;
        }
        else
        {

        }
    }
}

extern "C" void lcdMoveCursor(const Byte cAddress)
{
    cursor.position = static_cast<int>(cAddress);
}

extern "C" void lcdReturnHome()
{
    cursor.position = 0;
}

extern "C" void lcdSetContrast(const Word cContrast)
{
    contrast = cContrast;
}

extern "C" Word lcdGetContrast()
{
    return contrast;
}

extern "C" void lcdPrepareForSleep()
{
}

LcdMock::LcdMock()
{
	bBacklightIsOn = false;
	bLcdOn = false;
	screen.text = "idkfa iddqd";
	screen.position = 0;
	cursor.state = LcdMock::cCursor_off;
	cursor.position = 0;
	contrast = 0;
}

bool LcdMock::backlightIsOn()
{
	return bBacklightIsOn;
}

bool LcdMock::lcdIsOn()
{
	return bLcdOn;
}

LcdMock::cursor_t LcdMock::getCursor()
{
	return cursor;
}

LcdMock::screen_t LcdMock::getScreen()
{
    return screen;
}
