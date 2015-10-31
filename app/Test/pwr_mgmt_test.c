/*
 * pwr_mgmt_test.c
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#include "pwr_mgmt_test.h"
#include "pwr_mgmt.h"
#include "kbi.h"
#include "display.h"
#include "timer.h"
#include "buttons.h"

static kbiConfig_t kbiCfg;
static Bool bAwake = FALSE;

static test_pwrMgmt_isrCallback()
{
    displayBackLightOn(TRUE);
}

void test_pwrMgmt_sleep_init()
{
    displayOnOffControl_t setting;
    
    pwrMgmtInit();
    timersInit();
    buttonsInit();
    
    kbiCfg.bPullUp = TRUE;
    kbiCfg.bRisingEdge = FALSE;
    kbiConfigure(cKbiIn_P2,kbiCfg);
    kbiInstallIsrCallback(&test_pwrMgmt_isrCallback);
    
    displayInit();
    displayClear();
    setting.bBlinkingCursor = FALSE;
    setting.bCursorOn = FALSE;
    setting.bDisplayOn = TRUE;
    displayOnOffControl(setting);
    displayWrite("cierna - uspi");
    displayMoveCursor(cDisplayAddressMax/2);
    displayWrite("cervena - zobud");
    displayBackLightOn(TRUE);
}

void test_pwrMgmt_sleep_run()
{
    if (buttonIsPressed(cButton_Upper))
    {        
        displayBackLightOn(FALSE);
        pwrMgmtGoToSleep(TRUE);
    }
}

void test_pwrMgmt_recentReset_init()
{
    displayOnOffControl_t setting;
    pwrMgmtRecentresetSource_t source;
    
    timersInit();
    displayInit();
    buttonsInit();
    pwrMgmtInit();
    
    pwrMgmtGetRcentResetSource(&source);
    displayClear();
    setting.bBlinkingCursor = FALSE;
    setting.bCursorOn = FALSE;
    setting.bDisplayOn = TRUE;
    displayOnOffControl(setting);
    displayWrite("posledny reset:");
    displayMoveCursor(cDisplayAddressMax/2);
    if (cPwrMgmtResetSource_powerOn == source) 
    {
        displayWrite("nabeh napajania");
    } 
    else if (cPwrMgmtResetSource_watchdog == source)
    {
        displayWrite("watchdog");
    }
    else if (cPwrMgmtResetSource_lowVoltage == source)
    {
        displayWrite("slaba bateria");
    }
    else if (cPwrMgmtResetSource_illegalOpCode == source)
    {
        displayWrite("nepovolena oper.");
    }
    else if (cPwrMgmtResetSource_illegalAddress == source)
    {
        displayWrite("nepovolena addr.");
    }
    else if (cPwrMgmtResetSource_external == source)
    {
        displayWrite("externy pin");
    }
    else if (cPwrMgmtResetSource_unknown == source)
    {
        displayWrite("neznamy");
    }
    else
    {
        displayWrite("fatal error!");
    }
    displayBackLightOn(TRUE);
}

void test_pwrMgmt_recentReset_run()
{
    static Word* pIllegalAddress;

    if (buttonIsPressed(cButton_Lower)) 
    {
        waitX100us(0xffff);
    } 
    else if (buttonIsPressed(cButton_Upper))
    {
        pIllegalAddress = (Word*)6224;//0x1850;
        *pIllegalAddress = 1;
    }
}
