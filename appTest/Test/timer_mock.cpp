/*
 * timer_mock.cpp
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#include "timer_mock.h"
#include "timer.h"

static bool bRunning = false;

extern "C" void timerRestart(const Word cTimeout_ms)
{
	bRunning = true;
}

TimerMock::TimerMock()
{
	bRunning = false;
}

bool TimerMock::isRunning()
{
	return bRunning;
}

void TimerMock::stop()
{
	bRunning = false;
}

extern "C" Bool timerElapsedMiliSec()
{
	return !bRunning;
}
