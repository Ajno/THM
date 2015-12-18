/*
 * humidity.h
 *
 *  Created on: Dec 14, 2015
 *      Author: z0037ysj
 */

#ifndef HUMIDITY_H_
#define HUMIDITY_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

Word humidityRead(const sWord cTemperature, const Word cSamplingFrequency);

#ifdef __cplusplus
}
#endif
#endif /* HUMIDITY_H_ */
