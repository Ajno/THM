/*
 * kbi.h
 *
 *  Created on: Aug 18, 2015
 *      Author: z0037ysj
 */

#ifndef KBI_H_
#define KBI_H_

#include <stdtypes.h>

typedef enum
{
	cKbiIn_P1	= 0,
	cKbiIn_P2	= 1,
	cKbiIn_P3	= 2
}kbiPin_t;

typedef struct
{
	Bool bRisingEdge; // rising edge? otherwise falling edge
	Bool bPullUp; // enable internal pullup resistor for the pin?
}kbiConfig_t;

typedef void (*pKbiInterruptCallback_t)(void);

void kbiConfigure(const kbiPin_t idx, const kbiConfig_t config);
void kbiDisable();
void kbiInstallIsrCallback(pKbiInterruptCallback_t pcIsrClbck);

#endif /* KBI_H_ */
