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

void cyclic()
{
    
}

void baseInitApp()
{
    baseInstallApp(&cyclic);
}
