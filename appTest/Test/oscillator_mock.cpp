/*
 * oscillator_mock.cpp
 *
 *  Created on: 15. 12. 2015
 *      Author: z0037ysj
 */

#include <oscillator_mock.h>
#include <oscillator.h>

static Word frequency = 0;

extern "C" Word oscillatorReadAndClear()
{
    return frequency;
}

OscillatorMock::OscillatorMock(const Word cFreq)
{
    frequency = cFreq;
}

OscillatorMock::~OscillatorMock()
{
    // TODO Auto-generated destructor stub
}

