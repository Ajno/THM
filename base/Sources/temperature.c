/*
 * temperature.c
 *
 *  Created on: Nov 5, 2015
 *      Author: z0037ysj
 */

#include "adc.h"
#include "temperature.h"

static const sWord cScale = 10;
static const sWord cQ = -5000;
static const sWord cK = 32;

/*
 * k = 0,32258065
 * q = -50
 * 
 * return temperature in [deg x 10]
 */
sWord temperatureRead()
{
    sWord ret = (cK * adcChannelSample(cAdcChnnl_A3) + cQ);
    return (ret/cScale);
}

void temperatureInit()
{
    adcConfigure();
}
