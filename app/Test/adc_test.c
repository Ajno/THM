/*
 * adc_test.c
 *
 *  Created on: Nov 12, 2015
 *      Author: z0037ysj
 */

#include "adc_test.h"
#include "adc.h"

void test_adc_init()
{
    adcConfigure();
}

void test_adc_run()
{
    static Word tempSensorExternal = 0;
    static Word tempSensorInternal = 0;
    static Word vdd = 0;
    
    tempSensorExternal = adcSampleChannel(cAdcChnnl_A3);
    tempSensorInternal = adcSampleChannel(cAdcChnnl_tempSensor);
    vdd = adcSampleChannel(cAdcChnnl_vdd);
}
