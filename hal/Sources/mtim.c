/*
 * mtim.c
 *
 *  Created on: Nov 16, 2015
 *      Author: z0037ysj
 */

#include <derivative.h>
#include "mtim.h"
#include <stdtypes.h>

static Bool bIsrClbckInstalled = FALSE;
static pMtimInterruptCallback_t pIsrClbck;

void mtimResetCounter()
{
    MTIMSC_TRST = 1; // start the counter
}

Byte mtimReadCounter()
{
    return MTIMCNT;
}

void mtimInstallIsrCallback(pMtimInterruptCallback_t const pcIsrClbck)
{
    if (0 != pcIsrClbck)
    {
        pIsrClbck = pcIsrClbck;
        bIsrClbckInstalled = TRUE;
    }
}

void mtimConfigure()
{
    MTIMCLK_CLKS = 3; // External source (TCLK pin), rising edge
    MTIMSC_TOIE = 1; // interrupts are enabled
    MTIMSC_TSTP = 0; // start timer
    mtimResetCounter();
}

static void __interrupt VectorNumber_Vmtim isr_timerOverflow(void)
{
    if (bIsrClbckInstalled)
    {
        (*pIsrClbck)();
    }
    
    (void)(MTIMSC == 0x00); // Overflow int. flag clearing (first part) 
    MTIMSC_TOF=0;// Overflow int. flag clearing (second part) 
}
