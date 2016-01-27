/*
 * humidity.c
 *
 *  Created on: Dec 14, 2015
 *      Author: z0037ysj
 */

#include "humidity.h"
#include "oscillator.h"

/*
 * freq vs RH table for 25 deg
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
{ 61789, 95 }};

static Word freq2hum(const Word frequency)
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

Word humidityRead(const sWord cTemperature, const Word cSamplingFrequency)
{
    Word ret = oscillatorReadAndClear();
    ret = freq2hum(ret * cSamplingFrequency);
    return ret;
}
