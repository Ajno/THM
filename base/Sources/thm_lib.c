/*
 * thm_lib.c
 *
 *  Created on: Nov 4, 2015
 *      Author: z0037ysj
 */

#include "thm_lib.h"

static enum digits
{
    cNumOfDigits = 6,
};

char* thmLibItoa(const sWord cNum)
{
    static char buffer[cNumOfDigits + 1] =
    { 0 };
    Byte i = cNumOfDigits;
    sWord num = cNum;
    Bool bNegative = FALSE;

    if (0 > num)
    {
        num = -num;
        bNegative = TRUE;
    }

    do
    {
        buffer[--i] = num % 10 + '0';
        num /= 10;
    } while (num != 0);

    if (bNegative)
    {
        buffer[--i] = '-';
    }

    return &buffer[i];
}

sWord thmLibMovAvgFilter(const sWord in, sWord pBuff[], const Byte buffLen)
{
    static Bool bFirstCall = TRUE;

    if (bFirstCall)
    {
        Byte  idx;
        for (idx = 0; idx < buffLen; ++idx)
        {
            pBuff[idx] = in;
        }
        bFirstCall = FALSE;
    }
    else
    {
        Byte idx;
        // shift values in buffer
        for (idx = (buffLen - 1); idx > 0 ; --idx) 
        {
            pBuff[idx] = pBuff[idx - 1];
        }
        pBuff[0] = in;
    }

    // calculate moving average
    Byte idx;
    sWord out = 0;
    for (idx = 0; idx < buffLen; ++idx) 
    {
        out += pBuff[idx];
    }       
    return out/buffLen;
}
