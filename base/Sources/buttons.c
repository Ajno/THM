/*
 * buttons.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: Jano
 */

#include "buttons.h"
#include "io.h"

static const io_t cButtonIdx2ioIdx[2] = {cPin_A1, cPin_A2};

Bool buttonIsPressed(const buttons_t cButton)
{
    Bool bInput = FALSE;

    ioRead(cButtonIdx2ioIdx[cButton], &bInput);

    return (!bInput);
}

void buttonsInit()
{
    ioConfig_t config;
    config.bOutput = FALSE;
    config.bPullUp = TRUE;

    ioConfigure(cButtonIdx2ioIdx[cButton_Lower], config);
    ioConfigure(cButtonIdx2ioIdx[cButton_Upper], config);
}
