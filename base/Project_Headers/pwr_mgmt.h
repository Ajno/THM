/*
 * pwr_mgmt.h
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#ifndef PWR_MGMT_H_
#define PWR_MGMT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

typedef void (*pWakeUpCallback_t)(void);

typedef enum
{
    cPwrMgmtResetSource_unknown,
    cPwrMgmtResetSource_powerOn,
    cPwrMgmtResetSource_watchdog,
    cPwrMgmtResetSource_illegalOpCode,
    cPwrMgmtResetSource_illegalAddress,
    cPwrMgmtResetSource_external,
    cPwrMgmtResetSource_lowVoltage
}pwrMgmtRecentresetSource_t;

void pwrMgmtInit(pWakeUpCallback_t pWakeUpCallback);
void pwrMgmtGoToSleep(const Bool bDeepSleep);
void pwrMgmtGetRecentResetSource(pwrMgmtRecentresetSource_t* pSource);
Bool pwgMgmtIsLowBattery();

#ifdef __cplusplus
}
#endif
#endif /* PWR_MGMT_H_ */
