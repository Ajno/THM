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
static DisplayMock::cursorState_t cursorState = DisplayMock::cCursor_off;
static string screen = "";

extern "C" void displayBackLightOn(const Bool bBackLightOn)
{
	bBacklightIsOn = bBackLightOn;
}

extern "C" void displayOnOffControl(const displayOnOffControl_t cControl)
{
	bDisplayOn = cControl.bDisplayOn;
	if (cControl.bBlinkingCursorOn && cControl.bCursorOn)
	{
		cursorState = DisplayMock::cCursor_blinking;
	}
	else if (cControl.bCursorOn)
	{
		cursorState = DisplayMock::cCursor_on;
	}
	else
	{
		cursorState = DisplayMock::cCursor_off;
	}
}

extern "C" void displayClear()
{
    screen = "";
}

extern "C" void displayWrite(const char* pString)
{
    string str(pString);
    screen += str;
}

DisplayMock::DisplayMock()
{
	bBacklightIsOn = false;
	bDisplayOn = false;
	cursorState = DisplayMock::cCursor_off;
	screen = "idkfa iddqd";
}

bool DisplayMock::backlightIsOn()
{
	return bBacklightIsOn;
}

bool DisplayMock::displayIsOn()
{
	return bDisplayOn;
}

DisplayMock::cursorState_t DisplayMock::getCursorState()
{
	return cursorState;
}

string DisplayMock::getScreen()
{
    return screen;
}
