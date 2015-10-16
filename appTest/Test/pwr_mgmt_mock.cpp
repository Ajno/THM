/*
 * pwr_mgmt_mock.cpp
 *
 *  Created on: 7. 10. 2015
 *      Author: z0037ysj
 */

#include <pwr_mgmt_mock.h>
#include <pwr_mgmt.h>

static bool bSleeping = false;

extern "C" void pwrMgmtGoToSleep(const Bool bDeepSleep)
{
    bSleeping = true;
}

PwrMgmtMock::PwrMgmtMock()
{
    bSleeping = false;
}

PwrMgmtMock::~PwrMgmtMock()
{
    // TODO Auto-generated destructor stub
}

bool PwrMgmtMock::isSleeping()
{
    return bSleeping;
}

void PwrMgmtMock::wakeUp()
{
    bSleeping = false;
}
