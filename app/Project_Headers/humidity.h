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

Word humFreq2Humidity(const Word cFreq, const sWord cTemp);

#ifdef __cplusplus
}
#endif
#endif /* HUMIDITY_H_ */
