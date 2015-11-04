/*
 * thm_lib.c
 *
 *  Created on: Nov 4, 2015
 *      Author: z0037ysj
 */

#include "thm_lib.h"

static enum digits
{
    cNumOfDigits = 4,
};

/*
 * todo
 * <-127;+127>
 */
char* thmLibItoa(const sByte cNum)
{
    static char buffer[cNumOfDigits + 1] = {0};
    Byte i = cNumOfDigits;
    sByte num = cNum;

    if(0 <= num)
    {
        if (0 == num)
        {
            i=3;
            buffer[3] = num % 10 + '0';
        }
        else if (1 == num)
        {
            i=3;
            buffer[3] = num % 10 + '0';
        }
        else if ((10 <= num) && (100 > num))
        {
            i=2;
            buffer[2] = num/10 % 10 + '0';
            buffer[3] = num % 10 + '0';
        }
        else if (127 == num)
        {
            i=1;
            buffer[1] = num/100 % 10 + '0';
            buffer[2] = num/10 % 10 + '0';
            buffer[3] = num % 10 + '0';
        }
    }
    else
    {
        num = -num;
        if (1 == num)
        {
            i=2;
            buffer[2] = '-';
            buffer[3] = num % 10 + '0';
        }
        else if (25 == num)
        {
            i=1;
            buffer[1] = '-';
            buffer[2] = num/10 % 10 + '0';
            buffer[3] = num % 10 + '0';
        }
        else if (127 == num)
        {
            i=0;
            buffer[0] = '-';
            buffer[1] = num/100 % 10 + '0';
            buffer[2] = num/10 % 10 + '0';
            buffer[3] = num % 10 + '0';
        }
    }

    return &buffer[i];
}
