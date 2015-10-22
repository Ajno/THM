/*
 * timer_mock.cpp
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#include "timer_mock.h"
#include "timer.h"

static bool bRunningMiliSec = false;
static bool bRunningSec = false;

extern "C" void timerRestartMiliSec(const Word cTimeout_ms)
{
	bRunningMiliSec = true;
}

extern "C" void timerRestartSec(const Word cTimeout_sec)
{
	bRunningSec = true;
}

TimerMock::TimerMock()
{
	bRunningMiliSec = false;
	bRunningSec = false;
}

bool TimerMock::isRunning(const timerType_t cType)
{
	if (cTimerMiliSec == cType)
	{
		return bRunningMiliSec;
	}
	else if (cTimerSec == cType)
	{
		return bRunningSec;
	}
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

extern "C" Bool timerElapsedSec()
{
	return !bRunningSec;
}
