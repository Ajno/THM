/*
 * adc.h
 *
 *  Created on: Nov 5, 2015
 *      Author: z0037ysj
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdtypes.h>

typedef enum
{
    cAdcChnnl_A3            = 3,
    cAdcChnnl_tempSensor    = 26,
    cAdcChnnl_vdd           = 29
}adcChannel_t;

void adcConfigure();
Word adcSampleChannel(const adcChannel_t cChnnl);

#endif /* ADC_H_ */
