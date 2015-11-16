/*
 * mtim.c
 *
 *  Created on: Nov 16, 2015
 *      Author: z0037ysj
 */

#include <derivative.h>
#include "mtim.h"

void mtimConfigure()
{
    MTIMCLK_CLKS = 3; // External source (TCLK pin), rising edge
    //todo
}
