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
 * {f,quotient,offset,cScale1}
 * RH=(quotient*f/cScale1+offset)/cScale2
 */
static Word freqVsHum[10][4] =
{
{ 21, 780, 167, 491}, //(0;20)
{ 84, 780, 167, 491 }, //(20;30) quot=1.5873015873, off=166.666666667
{ 300, 215, 261, 464 }, //(30;40) quot=0.4629629629, off=261.11111113
{ 1050, 62, 360, 465 }, //(40;50) quot=0.1333333333, off=360
{ 1750, 32, 425, 448 }, //(50;55) quot=0.0714285714, off=425.00000005
{ 3232, 11, 491, 326 }, //(55;60) qout=0.0337381916, off=490.958164749
{ 9549, 6, 549, 379 }, //(60;70) qout=0.0158302991, off=548.836473009
{ 21008, 3, 617, 344 }, //(70;80) qout=0.0087267649, off=616.668121128
{ 45670, 1, 715, 247 }, //(80;90) qout=0.0040548211, off=714.8163166
{ 61789, 1, 758, 322 } }; //(90;100) qout=0.0031019294, off=758.334884298
static const Word cScale2 = 10;
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
    Byte idx = 0;

    if (freqVsHum[3][idx] >= frequency)
    {
        if (freqVsHum[1][idx] >= frequency)
        {
            if (freqVsHum[0][idx] >= frequency)
            {
                idx = 0;
            }
            else
            {
                idx = 1;
            }
        }
        else
        {
            if (freqVsHum[2][idx] >= frequency)
            {
                idx = 2;
            }
            else
            {
                idx = 3;
            }
        }
    }
    else
    {
        if (freqVsHum[6][idx] >= frequency)
        {
            if (freqVsHum[5][idx] >= frequency)
            {
                if (freqVsHum[4][idx] >= frequency)
                {
                    idx = 4;
                }
                else
                {
                    idx = 5;
                }
            }
            else
            {
                idx = 6;
            }
        }
        else
        {
            if (freqVsHum[7][idx] >= frequency)
            {
                idx = 7;
            }
            else
            {
                if (freqVsHum[8][idx] >= frequency)
                {
                    idx = 8;
                }
                else
                {
                    idx = 9;
                }
            }
        }
    }

    // RH=(frequency*quotient/cScale1+offset)/scale_2
    return ((frequency * freqVsHum[idx][1]) / freqVsHum[idx][3]
            + freqVsHum[idx][2]) / cScale2;
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
