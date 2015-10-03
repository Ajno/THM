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
	TimerMock();
	~TimerMock(){}
	void stop();
	bool isRunning();
private:
};

#endif /* TIMER_MOCK_H_ */
