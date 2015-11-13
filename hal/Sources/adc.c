/*
 * adc.c
 *
 *  Created on: Nov 5, 2015
 *      Author: z0037ysj
 */

#include <derivative.h>
#include "adc.h"

void adcConfigure()
{
//    1. Update the configuration register (ADCCFG) to select the input clock source and the divide ratio
//    used to generate the internal clock, ADCK. This register is also used for selecting sample time and
//    low-power configuration.
    ADCCFG_ADICLK = 1; // Bus clock divided by 2
    ADCCFG_ADIV = 0; // divide ration 1
    ADCCFG_MODE = 2; // 10 bit adc mode
    ADCCFG_ADLPC = 1;// low power configuration
    ADCCFG_ADLSMP = 1; // long sample time
//    2. Update status and control register 2 (ADCSC2) to select the conversion trigger (hardware or
//    software) and compare function options, if enabled.
    ADCSC2_ADTRG = 0; // SW trigger
    // compare function disabled by default
//    3. Update status and control register 1 (ADCSC1) to select whether conversions will be continuous
//    or completed only once, and to enable or disable conversion complete interrupts. The input channel
//    on which conversions will be performed is also selected here.
    ADCSC1_ADCH = cAdcChnnl_A3;// default adc channel
    APCTL1_ADPC3 = 1; // IO control for pin A3 disabled
}

/*
 * sampling time = 43 ADCK cycles + 5 bus clock cycles
 * i.e. 22,75 micro seconds when busclk = 4MHz
 */
Word adcChannelSample(const adcChannel_t cChnnl)
{
    volatile Word ret = 0;
    
    ADCSC1_ADCH = cChnnl;
    while (0 == ADCSC1_COCO)
    {
        ;// wait for end of conversion
    }
    
    ret = (ADCR);
    return ret;
}
