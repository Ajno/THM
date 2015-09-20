/*
 * test.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#include "display_test.h"
#include "io_test.h"

void baseInitApp()
{
    test_display_contrast_init();
    baseInstallApp(&test_display_contrast_run);
}
