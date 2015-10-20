/*
 * display_test.h
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#ifndef DISPLAY_TEST_H_
#define DISPLAY_TEST_H_

#include <string>
#include <display.h>
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

    typedef struct
    {
        cursorState_t   state;
        int             position;
    }cursor_t;

    typedef struct
    {
        string  text;
        int     position;
    }screen_t;


	DisplayMock();
	bool backlightIsOn();
	bool displayIsOn();
	cursor_t getCursor();
	screen_t getScreen();
private:
};

#endif /* DISPLAY_TEST_H_ */
