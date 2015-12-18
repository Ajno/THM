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
    Word ret = oscillatorReadAndClear();
    ret = ret * cSamplingFrequency; 
    return ret;
}
