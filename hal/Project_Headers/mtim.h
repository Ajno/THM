/*
 * mtim.h
 *
 *  Created on: Nov 16, 2015
 *      Author: z0037ysj
 */

#ifndef MTIM_H_
#define MTIM_H_

#include <stdtypes.h>

typedef void (*pMtimInterruptCallback_t)(void);

void mtimConfigure();
void mtimResetCounter();
Byte mtimReadCounter();
void mtimInstallIsrCallback(pMtimInterruptCallback_t const pcIsrClbck);

#endif /* MTIM_H_ */
