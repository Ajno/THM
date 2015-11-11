/*
 * timer_mock.cpp
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#include "timer_mock.h"
#include "timer.h"

static bool bRunningMiliSec = false;
static bool bRunningMiliSecX100 = false;
static bool bRunningSec = false;

extern "C" void timerRestartMiliSec(const Word cTimeout_ms)
{
	bRunningMiliSec = true;
}

extern "C" void timerRestartMiliSecX100(const Word cTimeout_msX100)
{
    bRunningMiliSecX100 = true;
}

extern "C" void timerRestartSec(const Word cTimeout_sec)
{
	bRunningSec = true;
}

TimerMock::TimerMock()
{
	bRunningMiliSec = false;
	bRunningMiliSecX100 = false;
	bRunningSec = false;
}

bool TimerMock::isRunning(const timerType_t cType)
{
    bool ret = false;

    switch (cType)
    {
        case cTimerMiliSec:
            ret = bRunningMiliSec;
            break;
        case cTimerMiliSecX100:
            ret = bRunningMiliSecX100;
            break;
        case cTimerSec:
            return bRunningSec;
            break;
        default:
            break;
    }

    return ret;
}

void TimerMock::stop(const timerType_t cType)
{
	if (cTimerMiliSec == cType)
	{
		bRunningMiliSec = false;
	}
	else if (cTimerSec == cType)
	{
		bRunningSec = false;
	}
}

extern "C" Bool timerElapsedMiliSec()
{
	return !bRunningMiliSec;
}

extern "C" Bool timerElapsedMiliSecX100()
{
    return !bRunningMiliSecX100;
}

extern "C" Bool timerElapsedSec()
{
	return !bRunningSec;
}
