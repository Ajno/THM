/*
 * oscillator.c
 *
 *  Created on: Nov 20, 2015
 *      Author: z0037ysj
 */

#include "oscillator.h"
#include "mtim.h"

static const Word cInc = 0x0100; // 256
static const Word cLimit = 0xff00; // 65280
static Word oscillator = 0;
static Bool bClearOscillator = FALSE;

/*
 * read number of oscillations since last read,
 * bandwidth = 1,1 sec 
 * read the number of oscillations at least 
 * once per 1,1 sec to prevent undersampling
 */
Word oscillatorReadAndClear()
{
    Word oscillatorRet = oscillator;
    
    if (bClearOscillator)
    {
        oscillatorRet = 0;
    }
    oscillatorRet += mtimReadCounter();        
    bClearOscillator = TRUE;    
    
    return oscillatorRet;
}

void counterIsrCallback()
{
    if ((bClearOscillator) || (cLimit <= oscillator))
    {
        oscillator = cInc;
        bClearOscillator = FALSE;
    }
    else
    {
        oscillator += cInc;        
    }
}

void oscillatorInit()
{
    mtimConfigure();// counter is started here
    oscillator = 0;
    mtimInstallIsrCallback(&counterIsrCallback);
}
