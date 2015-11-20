/*
 * oscillator.h
 *
 *  Created on: Nov 20, 2015
 *      Author: z0037ysj
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

void oscillatorInit();
/*
 * read number of oscillations since last read,
 * bandwidth = 1,1 sec 
 * read the number of oscillations at least 
 * once per 1,1 sec to prevent undersampling
 */
Word oscillatorRead();

#ifdef __cplusplus
}
#endif
#endif /* OSCILLATOR_H_ */
