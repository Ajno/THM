/*
 * timer.h
 *
 *  Created on: Aug 24, 2015
 *      Author: z0037ysj
 */

#ifndef TIMER_H_
#define TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

void timersInit();
void wait500ns();
void waitX100us(const Word cDelay);
void timerRestartMiliSec(const Word cTimeout_ms);
void timerRestartSec(const Word cTimeout_sec);
Bool timerElapsedMiliSec();
Bool timerElapsedSec();

#ifdef __cplusplus
}
#endif
#endif /* TIMER_H_ */
