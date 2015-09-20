/*
 * kbi.c
 *
 *  Created on: Aug 18, 2015
 *      Author: z0037ysj
 */

#include "kbi.h"
#include "io.h"
#include <derivative.h>

static Bool bIsrClbckInstalled = FALSE;
static pKbiInterruptCallback_t pIsrClbck;

static const Byte cKbiIn2PinEnableMask[cKbiIn_P3 + 1] = 
{
	KBIPE_KBIPE1_MASK,
	KBIPE_KBIPE2_MASK,
	KBIPE_KBIPE3_MASK,
};

static const Byte cKbiIn2EdgeSelectMask[cKbiIn_P3 + 1] =
{
	KBIES_KBEDG1_MASK,
	KBIES_KBEDG2_MASK,
	KBIES_KBEDG3_MASK,
};

static const Byte cKbiIn2PinIdx[cKbiIn_P3 + 1] =
{
	cPin_A1,// cKbiIn_P1
	cPin_A2,// cKbiIn_P2
	cPin_A3// cKbiIn_P3
};

void kbiConfigure(const kbiPin_t idx, const kbiConfig_t config)
{
	ioConfig_t pin;
	pin.bPullUp = config.bPullUp;
	pin.bOutput = FALSE;
	//	1. Mask keyboard interrupts by clearing KBIE in KBISC.
	KBISC = 0;
	//	2. Enable the KBI polarity by setting the appropriate KBEDGn bits in KBIES.
	if (config.bRisingEdge) 
	{
		KBIES |= cKbiIn2EdgeSelectMask[idx];
	}
	//	3. If using internal pullup/pulldown device, configure the associated pullup enable bits in PTxPE.
	ioConfigure(cKbiIn2PinIdx[idx],pin);
	//	4. Enable the KBI pins by setting the appropriate KBIPEn bits in KBIPE.
	KBIPE |= cKbiIn2PinEnableMask[idx];
	//	5. Write to KBACK in KBISC to clear any false interrupts.
	KBISC_KBACK = 1;
	if (0 == KBISC_KBIE) 
	{	
		//	6. Set KBIE in KBISC to enable interrupts.
		KBISC_KBIE = 1;
	}
}

void kbiInstallIsrCallback(pKbiInterruptCallback_t pcIsrClbck)
{
	if (0 != pcIsrClbck)
	{
		pIsrClbck = pcIsrClbck;
		bIsrClbckInstalled = TRUE;
	}
}

void kbiDisable()
{
	// Mask keyboard interrupts by clearing KBIE in KBISC.
	KBISC = 0;
	// Disable the KBI pins
	KBIPE = 0;
}

static void __interrupt VectorNumber_Vkeyboard isr_keyboardInterrupt(void)
{
	if (bIsrClbckInstalled)
	{
		(*pIsrClbck)();
	}
	
	KBISC_KBACK = 1;// flag clearing
}
