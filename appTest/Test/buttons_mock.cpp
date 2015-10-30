/*
 * buttons_mock.cpp
 *
 *  Created on: 7. 10. 2015
 *      Author: z0037ysj
 */

#include <buttons_mock.h>
#include <buttons.h>

static buttonState_t buttonState[2];

extern "C" void buttonStateDetection(const buttons_t cButton, buttonState_t* pState)
{
    *pState = buttonState[cButton];
}

ButtonsMock::ButtonsMock()
{
    buttonState[cButton_Lower] = cButtonState_Released;
    buttonState[cButton_Upper] = cButtonState_Released;
}

ButtonsMock::~ButtonsMock()
{
    // TODO Auto-generated destructor stub
}

void ButtonsMock::setState(const buttonState_t cState, const buttons_t cButton)
{
    buttonState[cButton] = cState;
}
