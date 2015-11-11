/*
 * timer_mock.h
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#ifndef TIMER_MOCK_H_
#define TIMER_MOCK_H_

class TimerMock
{
public:
	typedef enum
	{
		cTimerMiliSec,
		cTimerMiliSecX100,
		cTimerSec
	}timerType_t;
	TimerMock();
	~TimerMock(){}
	void stop(const timerType_t cType);
	bool isRunning(const timerType_t cType);
private:
};

#endif /* TIMER_MOCK_H_ */
