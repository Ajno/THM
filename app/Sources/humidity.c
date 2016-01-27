/*
 * humidity.c
 *
 *  Created on: Dec 14, 2015
 *      Author: z0037ysj
 */

#include "humidity.h"
#include "oscillator.h"

/*
 * f[Hz] vs RH[%] - for 25 deg
 */
static Word freqVsHum[10][2] =
{
{ 21, 20 },
{ 84, 30 },
{ 300, 40 },
{ 1050, 50 },
{ 1750, 55 },
{ 3232, 60 },
{ 9549, 70 },
{ 21008, 80 },
{ 45670, 90 },
{ 61789, 95 } };

/*
 * t[10xdeg] vs RH[%] error relative to reference 25 deg
 */
static sWord tempVsHumErr[3][2] =
{
{ 50, 10 },
{ 150, 5 },
{ 300, -2 } };

static Word frequency2humidity(const Word frequency)
{
    Word ret = 0;

    if (freqVsHum[3][0] >= frequency)
    {
        if (freqVsHum[1][0] >= frequency)
        {
            if (freqVsHum[0][0] >= frequency)
            {
                ret = freqVsHum[0][1];
            }
            else
            {
                ret = freqVsHum[1][1];
            }
        }
        else
        {
            if (freqVsHum[2][0] >= frequency)
            {
                ret = freqVsHum[2][1];
            }
            else
            {
                ret = freqVsHum[3][1];
            }
        }
    }
    else
    {
        if (freqVsHum[6][0] >= frequency)
        {
            if (freqVsHum[5][0] >= frequency)
            {
                if (freqVsHum[4][0] >= frequency)
                {
                    ret = freqVsHum[4][1];
                }
                else
                {
                    ret = freqVsHum[5][1];
                }
            }
            else
            {
                ret = freqVsHum[6][1];
            }
        }
        else
        {
            if (freqVsHum[7][0] >= frequency)
            {
                ret = freqVsHum[7][1];
            }
            else
            {
                if (freqVsHum[8][0] >= frequency)
                {
                    ret = freqVsHum[8][1];
                }
                else
                {
                    ret = freqVsHum[9][1];
                }
            }
        }
    }

    return ret;
}

static void compensate4Temperature(const sWord cTemperature, Word* pHumidity)
{
    if (250 > cTemperature)
    {
        if (tempVsHumErr[0][0] > cTemperature)
        {
            *pHumidity += tempVsHumErr[0][1];
        }
        else if (tempVsHumErr[1][0] > cTemperature)
        {
            *pHumidity += tempVsHumErr[1][1];
        }
    }
    else
    {
        if (tempVsHumErr[2][0] < cTemperature)
        {
            *pHumidity += tempVsHumErr[2][1];
        }
    }
}

Word humidityRead(const sWord cTemperature, const Word cSamplingFrequency)
{
    Word humidity = oscillatorReadAndClear();
    humidity = frequency2humidity(humidity * cSamplingFrequency);
    compensate4Temperature(cTemperature, &humidity);
    return humidity;
}
