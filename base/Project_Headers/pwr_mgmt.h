/*
 * pwr_mgmt.h
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#ifndef PWR_MGMT_H_
#define PWR_MGMT_H_

#include <stdtypes.h>

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

void pwrMgmtInit();
void pwrMgmtGoToSleep(const Bool bDeepSleep);
void pwrMgmtGetRcentResetSource(pwrMgmtRecentresetSource_t* pSource);

#endif /* PWR_MGMT_H_ */
