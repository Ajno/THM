/*
 * pwm.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: z0037ysj
 */

#include "pwm.h"
#include <derivative.h>

static Bool bIsrClbckInstalled = FALSE;
static pPwmInterruptCallback_t pIsrClbck;

void pwmConfigureTimer(const pwmTimerConfig_t cfg)
{
	// clear register
	TPMSC = 0;
	// counter
	TPMSC_TOIE = cfg.bOverflowInterruptEnable;
	TPMSC_PS = cfg.prescaler;
	TPMSC_CLKSx = cfg.clock;
}

void pwmConfigureChannel(const pwmChannelConfig_t cfg)
{
	// channel 0
	TPMC0SC = TPMC0SC | (cfg.mode << TPMC0SC_ELS0x_BITNUM);
//	TPMC0SC_CH0IE = cfg.chnnl0.bChannelInterruptEnable;
}

void pwmSetOverflowInterruptEnable(const Bool bEnable)
{
    TPMSC_TOIE = bEnable;
}

Word pwmReadTimer()
{
	return TPMCNT;
}

Word pwmReadModulo()
{
	return TPMMOD;
}

void pwmWriteModulo(const Word modulo)
{
	TPMMOD = modulo;
}

void pwmWriteChannel(const Word value)
{
	TPMC0V = value;
}

void pwmInstallTimerIsrCallback(pPwmInterruptCallback_t const pcIsrClbck)
{
	if (0 != pcIsrClbck)
	{
		pIsrClbck = pcIsrClbck;
		bIsrClbckInstalled = TRUE;
	}
}

void pwmClearTimer()
{
    TPMCNTH = 0;
}

static void __interrupt VectorNumber_Vtpmovf isr_timerOverflow(void)
{
	if (bIsrClbckInstalled)
	{
		(*pIsrClbck)();
	}
	
	(void)(TPMSC == 0x00); // Overflow int. flag clearing (first part) 
	TPMSC_TOF=0;// Overflow int. flag clearing (second part) 
}
