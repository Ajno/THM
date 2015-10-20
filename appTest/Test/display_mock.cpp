/*
 * display_test.cpp
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#include <display_mock.h>
#include <display.h>

static bool bBacklightIsOn = false;
static bool bDisplayOn = false;
static DisplayMock::screen_t screen = {"", 0};
static DisplayMock::cursor_t cursor = {DisplayMock::cCursor_off, 0};

extern "C" void displayBackLightOn(const Bool bBackLightOn)
{
	bBacklightIsOn = bBackLightOn;
}

extern "C" void displayOnOffControl(const displayOnOffControl_t cControl)
{
	bDisplayOn = cControl.bDisplayOn;
	if (cControl.bBlinkingCursorOn && cControl.bCursorOn)
	{
		cursor.state = DisplayMock::cCursor_blinking;
	}
	else if (cControl.bCursorOn)
	{
	    cursor.state = DisplayMock::cCursor_on;
	}
	else
	{
	    cursor.state = DisplayMock::cCursor_off;
	}
}

extern "C" void displayClear()
{
    screen.text = "";
    screen.position = 0;
    cursor.position = 0;
}

extern "C" void displayWrite(const char* pString)
{
    string str(pString);
    screen.text += str;
}

extern "C" void displayOrCursorShift(const displayMovingDirection_t cSetting)
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

DisplayMock::DisplayMock()
{
	bBacklightIsOn = false;
	bDisplayOn = false;
	screen.text = "idkfa iddqd";
	screen.position = 0;
	cursor.state = DisplayMock::cCursor_off;
	cursor.position = 0;
}

bool DisplayMock::backlightIsOn()
{
	return bBacklightIsOn;
}

bool DisplayMock::displayIsOn()
{
	return bDisplayOn;
}

DisplayMock::cursor_t DisplayMock::getCursor()
{
	return cursor;
}

DisplayMock::screen_t DisplayMock::getScreen()
{
    return screen;
}
