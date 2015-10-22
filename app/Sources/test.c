/*
 * test.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "display_test.h"
#include "timer_test.h"

void baseInitApp()
{
    test_timer_startStop_init();
    baseInstallApp(&test_timer_startStop_run);
}
