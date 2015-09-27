/*
 * buttons.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: Jano
 */

#include "buttons.h"
#include "io.h"

static buttonState_t bLowerState = cButtonState_Released;
static buttonState_t bUpperState = cButtonState_Released;

static const io_t cButtonIdx2ioIdx[2] = {cPinA1, cPinA2};

static Bool buttonIsPressed(const buttons_t cButton)
{
    Bool bInput = FALSE;

    ioRead(cButtonIdx2ioIdx[cButton], &bInput);

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

    ioConfigure(cButtonIdx2ioIdx[cButton_Lower], config);
    ioConfigure(cButtonIdx2ioIdx[cButton_Upper], config);

    bLowerState = cButtonState_Released;
    bUpperState = cButtonState_Released;
}
