/*
 * buttons_mock.cpp
 *
 *  Created on: 7. 10. 2015
 *      Author: z0037ysj
 */

#include <buttons_mock.h>
#include <buttons.h>

static bool buttonState[2];

extern "C" Bool buttonIsPressed(const buttons_t cButton)
{
    return buttonState[cButton];
}

ButtonsMock::ButtonsMock()
{
    buttonState[cButton_Lower] = false;
    buttonState[cButton_Upper] = false;
}

ButtonsMock::~ButtonsMock()
{
    // TODO Auto-generated destructor stub
}

void ButtonsMock::setState(const bool cState, const buttons_t cButton)
{
    buttonState[cButton] = cState;
}
