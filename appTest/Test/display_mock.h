/*
 * display_test.h
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#ifndef DISPLAY_TEST_H_
#define DISPLAY_TEST_H_

#include <string>
using namespace std;

class DisplayMock
{
public:
	typedef enum
	{
		cCursor_on,
		cCursor_blinking,
		cCursor_off
	}cursorState_t;

	DisplayMock();
	bool backlightIsOn();
	bool displayIsOn();
	cursorState_t getCursorState();
	string getScreen();
private:
};

#endif /* DISPLAY_TEST_H_ */
