/*
 * temperature.h
 *
 *  Created on: Nov 5, 2015
 *      Author: z0037ysj
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

void temperatureInit();
/*
 * @brief Reads the temperature
 *
 * returns temperature in tenth of degrees
 * e.g. 21,5 deg equals 215
 */
sWord temperatureRead();

#ifdef __cplusplus
}
#endif
#endif /* TEMPERATURE_H_ */
