/*
 * pwr_mgmt.c
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#include "pwr_mgmt.h"
#include "system.h"
#include "kbi.h"
#include "io.h"

void pwrMgmtInit()
{
    systemConfig_t cfg;
    
    cfg.bStopModeEn = TRUE;
    cfg.bLowVoltageDetectionEn = TRUE;
    cfg.bLowVoltageDetectionInStopModeEn = TRUE;
    cfg.bLowVoltageResetEn = TRUE;
    cfg.bLowVoltageTripPointHigh = TRUE;
    cfg.bLowVoltageWarningPointHigh = TRUE;
    systemConfigure(cfg);
    
    kbiInstallIsrCallback(&kbiDisable);
}

void pwrMgmtGoToSleep(const Bool bDeepSleep)
{
    kbiConfig_t cfgKbi;
    
    if (bDeepSleep) 
    {        
        // configure keyboard interrupt to make wake up possible
        cfgKbi.bPullUp = TRUE;
        cfgKbi.bRisingEdge = FALSE;
        kbiConfigure(cKbiIn_P1, cfgKbi);
        kbiConfigure(cKbiIn_P2, cfgKbi);
        systemStop();
    } 
    else 
    {
        // todo disable PWM interrupt, ...?
        systemWait();
    }
}

void pwrMgmtGetRecentResetSource(pwrMgmtRecentresetSource_t* pSource)
{
    systemRecentResetSource_t source;
    systemReadResetSource(&source);
          
    if (source.bPowerOnReset) 
    {
        *pSource = cPwrMgmtResetSource_powerOn;
    } 
    else if (source.bCOPWatchdog)
    {
        *pSource = cPwrMgmtResetSource_watchdog;
    }
    else if (source.bLowVoltageDetect)
    {
        *pSource = cPwrMgmtResetSource_lowVoltage;
    }
    else if (source.bIllegalOpcode)
    {
        *pSource = cPwrMgmtResetSource_illegalOpCode;
    }
    else if (source.bIllegalAddress)
    {
        *pSource = cPwrMgmtResetSource_illegalAddress;
    }
    else if (source.bExternalResetPin)
    {
        *pSource = cPwrMgmtResetSource_external;
    }
    else
    {
        *pSource = cPwrMgmtResetSource_unknown;
    }
}
