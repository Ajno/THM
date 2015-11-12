/*
 * test.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "adc_test.h"

void baseInitApp()
{
    test_adc_init();
    baseInstallApp(&test_adc_run);
}
