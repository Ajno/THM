/*
 * io_pins.h
 *
 *  Created on: Jul 1, 2015
 *      Author: z0037ysj
 */

#ifndef IO_PINS_H_
#define IO_PINS_H_

#include <stdtypes.h>

typedef enum
{
    cPin_A0 = 0,
	cPin_A1	= 1,
	cPin_A2	= 2,
	cPin_A3	= 3,
	cPin_B0	= 4,
	cPin_B1	= 5,
	cPin_B2	= 6,
	cPin_B3	= 7,
	cPin_B4	= 8,
	cPin_B5	= 9,
	cPin_B6	= 10,
	cPin_B7	= 11
} io_t;

typedef struct
{
	Bool bOutput; // output otherwise input?
	Bool bPullUp; // internal pull-up enabled?		
}ioConfig_t;

void ioConfigure(const io_t idx, const ioConfig_t config);
void ioConfigurePortB(const ioConfig_t config);
void ioWrite(const io_t idx, const Bool bValue);
void ioWritePortB(const Byte cValue);
void ioReadPortB(Byte* pValue);
void ioRead(const io_t idx, Bool* pValue);

#endif /* IO_PINS_H_ */
