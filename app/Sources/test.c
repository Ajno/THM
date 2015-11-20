/*
 * test.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "oscillator_test.h"

void baseInitApp()
{
    test_oscillator_init();
    baseInstallApp(&test_oscillator_run);
}
