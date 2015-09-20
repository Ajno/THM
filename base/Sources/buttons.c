/*
 * buttons.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: Jano
 */

#include "buttons.h"

static buttonState_t bLowerState = cButtonState_Released;
static buttonState_t bUpperState = cButtonState_Released;

static Bool buttonIsPressed(const buttons_t cButton)
{
    Bool bInput = FALSE;

    ioRead(cButton, &bInput);

    return (!bInput);
}

static buttonState_t buttonGetState(const buttons_t cButton)
{
    buttonState_t ret = cButtonState_Released;
    
    if (cButton_Lower == cButton)
    {
        ret = bLowerState;
    }
    else if (cButton_Upper == cButton)
    {
        ret = bUpperState;
    }
    
    return ret;
}

static void buttonSetState(const buttons_t cButton, const buttonState_t cState)
{        
    if (cButton_Lower == cButton)
    {
        bLowerState = cState;
    }
    else if (cButton_Upper == cButton)
    {
        bUpperState = cState;
    }
}

void buttonStateDetection(const buttons_t cButton, buttonState_t* pState)
{
    if (buttonIsPressed(cButton))
    {
        if (cButtonState_Released == buttonGetState(cButton))
        {
            *pState = cButtonState_JustPressed;
        }
        else
        {
            *pState = cButtonState_Pressed;
        }
    }
    else
    {
        *pState = cButtonState_Released;
    }

    buttonSetState(cButton,*pState);
}

void buttonsInit()
{
    ioConfig_t config;
    config.bOutput = FALSE;
    config.bPullUp = TRUE;

    ioConfigure(cButton_Lower, config);
    ioConfigure(cButton_Upper, config);

    bLowerState = cButtonState_Released;
    bUpperState = cButtonState_Released;
}
