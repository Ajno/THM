/*
 * buttons.h
 *
 *  Created on: Aug 6, 2015
 *      Author: z0037ysj
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "io.h"

typedef enum
{
    cButton_Lower = cPin_A2,
    cButton_Upper = cPin_A1
}buttons_t;

typedef enum
{
    cButtonState_Released,
    cButtonState_Pressed,
    cButtonState_JustPressed
}buttonState_t;

void buttonsInit();
void buttonStateDetection(const buttons_t cButton, buttonState_t* pState);

#endif /* BUTTONS_H_ */
