/*
 * oscillator.c
 *
 *  Created on: Nov 20, 2015
 *      Author: z0037ysj
 */

#include "oscillator.h"
#include "mtim.h"

typedef union
{
    Word value;
    Byte bytes[2];
}oscillator_t;

static oscillator_t oscillator;

/*
 * read number of oscillations since last read,
 * bandwidth = 1,1 sec 
 * read the number of oscillations at least 
 * once per 1,1 sec to prevent undersampling
 */
Word oscillatorRead()
{
    oscillator.bytes[0] = mtimReadCounter();
    return oscillator.value;
}

void counterIsrCallback()
{
    oscillator.bytes[0] = 0;
    if (255 > oscillator.bytes[1])
    {        
        oscillator.bytes[1]++;
    }
    else
    {
        oscillator.bytes[1] = 0;
    }
}

void oscillatorInit()
{
    mtimConfigure();// counter is started here
    oscillator.value = 0;
    mtimInstallIsrCallback(&counterIsrCallback);
}
