/*
 * app.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "base.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "pwr_mgmt.h"

#include "display_test.h"
#include "io_test.h"

void baseInitApp()
{
    test_display_shiftAndBackLight_init();
    baseInstallApp(&test_display_shiftAndBackLight_run);
}
