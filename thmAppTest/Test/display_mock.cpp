/*
 * display_test.cpp
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#include <display_mock.h>
#include <display.h>

static bool bBacklightIsOn = false;

extern "C" void displayBackLightOn(const Bool bBackLightOn)
{
	bBacklightIsOn = bBackLightOn;
}

DisplayMock::DisplayMock()
{
	bBacklightIsOn = false;
}

bool DisplayMock::backlightIsOn()
{
	return bBacklightIsOn;
}
