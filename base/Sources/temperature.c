/*
 * temperature.c
 *
 *  Created on: Nov 5, 2015
 *      Author: z0037ysj
 */

#include "adc.h"
#include "temperature.h"

static const sWord cK1 = 3;
static const sWord cK2 = 2;
static const sWord cK3 = 3;
static const sWord cQ1 = -500;
static const sWord cQ2 = -50;
static const sWord cQ3 = -5;
static const sWord cScale2 = 10;
static const sWord cScale3 = 100;

/*
 * temperature in degrees = number*k + q
 * k = 0,32258065
 * q = -50
 * 
 * return temperature in [deg x 10]
 */
sWord temperatureRead()
{
    sWord tempRaw = adcChannelSample(cAdcChnnl_A3);
    sWord ret = 0;
    ret = cK1 * tempRaw + cQ1 + (cK2 * tempRaw + cQ2)/cScale2 + (cK3 * tempRaw + cQ3)/cScale3;
    return ret;
}

void temperatureInit()
{
    adcConfigure();
}
