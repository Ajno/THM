/*
 * temperature.c
 *
 *  Created on: Nov 5, 2015
 *      Author: z0037ysj
 */

#include "adc.h"
#include "temperature.h"

sWord temperatureRead()
{
    return adcSampleChannel(cAdcChnnl_A3);
}

void temperatureInit()
{
    adcConfigure();
}
