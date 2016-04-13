/*
 * system.c
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#include "system.h"
#include <derivative.h>
#include <hidef.h>

void systemConfigure(const systemConfig_t cCfg)
{
    SOPT1_STOPE  = cCfg.bStopModeEn;
    SPMSC1_LVDE  = cCfg.bLowVoltageDetectionEn;
    SPMSC1_LVDSE = cCfg.bLowVoltageDetectionInStopModeEn;
    SPMSC1_LVDRE = cCfg.bLowVoltageResetEn;
    SPMSC3_LVDV  = cCfg.bLowVoltageTripPointHigh;
    SPMSC3_LVWV  = cCfg.bLowVoltageWarningPointHigh;
}

void systemResetWatchdog()
{
    __RESET_WATCHDOG(); /* feeds the dog */
}

void systemEnableInterrupts()
{
    EnableInterrupts;
}

void systemWait()
{
    _Wait;
}

void systemStop()
{
    _Stop;
}

Bool systemLowVoltageWarningDetected()
{
    return (SPMSC3_LVWF == 1);
}

void systemClearLowVoltageWarning()
{
    SPMSC3_LVWACK = 1;
}

void systemReadResetSource(systemRecentResetSource_t* pSource)
{
    pSource->bCOPWatchdog = (1 == SRS_COP);
    pSource->bExternalResetPin = (1 == SRS_PIN);
    pSource->bIllegalAddress = (1 == SRS_ILAD);
    pSource->bLowVoltageDetect = (1 == SRS_LVD);
    pSource->bPowerOnReset = (1 == SRS_POR);
    pSource->bIllegalOpcode = (1 == SRS_ILOP);
}
