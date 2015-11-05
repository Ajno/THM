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
//    2. Update status and control register 2 (ADCSC2) to select the conversion trigger (hardware or
//    software) and compare function options, if enabled.
//    3. Update status and control register 1 (ADCSC1) to select whether conversions will be continuous
//    or completed only once, and to enable or disable conversion complete interrupts. The input channel
//    on which conversions will be performed is also selected here.
}
