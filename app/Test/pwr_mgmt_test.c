/*
 * pwr_mgmt_test.c
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#include "pwr_mgmt_test.h"
#include "pwr_mgmt.h"
#include "kbi.h"
#include "lcd.h"
#include "timer.h"
#include "buttons.h"

static kbiConfig_t kbiCfg;
static Bool bAwake = FALSE;

static test_pwrMgmt_isrCallback()
{
    lcdBackLightOn(TRUE);
}

void test_pwrMgmt_sleep_init()
{
    lcdOnOffControl_t setting;
    
    pwrMgmtInit(0);
    timersInit();
    buttonsInit();
    
    kbiCfg.bPullUp = TRUE;
    kbiCfg.bRisingEdge = FALSE;
    kbiConfigure(cKbiIn_P2,kbiCfg);
    kbiInstallIsrCallback(&test_pwrMgmt_isrCallback);
    
    lcdInit();
    lcdClear();
    setting.bBlinkingCursor = FALSE;
    setting.bCursorOn = FALSE;
    setting.bLcdOn = TRUE;
    lcdOnOffControl(setting);
    lcdWrite("cierna - uspi");
    lcdMoveCursor(cLcdAddressMax/2);
    lcdWrite("cervena - zobud");
    lcdBackLightOn(TRUE);
}

void test_pwrMgmt_sleep_run()
{
    if (buttonIsPressed(cButton_Upper))
    {        
        lcdBackLightOn(FALSE);
        pwrMgmtGoToSleep(TRUE);
    }
}

void test_pwrMgmt_recentReset_init()
{
    lcdOnOffControl_t setting;
    pwrMgmtRecentresetSource_t source;
    
    timersInit();
    lcdInit();
    buttonsInit();
    pwrMgmtInit(0);
    
    pwrMgmtGetRcentResetSource(&source);
    lcdClear();
    setting.bBlinkingCursor = FALSE;
    setting.bCursorOn = FALSE;
    setting.bLcdOn = TRUE;
    lcdOnOffControl(setting);
    lcdWrite("posledny reset:");
    lcdMoveCursor(cLcdAddressMax/2);
    if (cPwrMgmtResetSource_powerOn == source) 
    {
        lcdWrite("nabeh napajania");
    } 
    else if (cPwrMgmtResetSource_watchdog == source)
    {
        lcdWrite("watchdog");
    }
    else if (cPwrMgmtResetSource_lowVoltage == source)
    {
        lcdWrite("slaba bateria");
    }
    else if (cPwrMgmtResetSource_illegalOpCode == source)
    {
        lcdWrite("nepovolena oper.");
    }
    else if (cPwrMgmtResetSource_illegalAddress == source)
    {
        lcdWrite("nepovolena addr.");
    }
    else if (cPwrMgmtResetSource_external == source)
    {
        lcdWrite("externy pin");
    }
    else if (cPwrMgmtResetSource_unknown == source)
    {
        lcdWrite("neznamy");
    }
    else
    {
        lcdWrite("fatal error!");
    }
    lcdBackLightOn(TRUE);
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
