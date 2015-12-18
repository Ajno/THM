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
static Word overflowCounter = 0;
static Bool bBlockOverflowCounter = FALSE;

/*
 * read number of oscillations since last read,
 * bandwidth = 1,1 sec 
 * read the number of oscillations at least 
 * once per 1,1 sec to prevent undersampling
 */
Word oscillatorReadAndClear()
{
    Word oscillatorRet;
    
    bBlockOverflowCounter = TRUE;
    
    oscillatorRet = mtimReadCounter();    
    mtimResetCounter();
    oscillatorRet += overflowCounter;
    overflowCounter = 0;
    
    bBlockOverflowCounter = FALSE;
    
    return oscillatorRet;
}

void counterIsrCallback()
{
    if (!bBlockOverflowCounter)
    {
        overflowCounter += cInc;
    }
}

void oscillatorInit()
{
    mtimConfigure();// counter is started here
    overflowCounter = 0;
    bBlockOverflowCounter = FALSE;
    mtimInstallIsrCallback(&counterIsrCallback);
}
