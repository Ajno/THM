/*
 * humidity.c
 *
 *  Created on: Dec 14, 2015
 *      Author: z0037ysj
 */

#include "humidity.h"
#include "oscillator.h"

Word humidityRead(const sWord cTemperature, const Word cSamplingFrequency)
{
    // fixme
    return oscillatorReadAndClear();
}
