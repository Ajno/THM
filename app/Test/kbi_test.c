/*
 * kbi_test.c
 *
 *  Created on: Aug 18, 2015
 *      Author: z0037ysj
 */

#include "kbi_test.h"

void test_kbi_init(const kbiPin_t pin)
{
	kbiConfig_t config;
	config.bPullUp = TRUE;
	config.bRisingEdge = FALSE;
	
	kbiConfigure(pin,config);
	kbiInstallIsrCallback(&kbiDisable);
}
