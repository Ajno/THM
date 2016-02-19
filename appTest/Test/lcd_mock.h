/*
 * display_test.h
 *
 *  Created on: 3.10.2015
 *      Author: Jano
 */

#ifndef LCD_TEST_H_
#define LCD_TEST_H_

#include <string>
#include <lcd.h>
using namespace std;


class LcdMock
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
       int    position;
    }cursor_t;

    typedef struct
    {
        string  text;
        int     position;
        int     contrast;
    }screen_t;

	LcdMock();
	bool backlightIsOn();
	bool lcdIsOn();
	cursor_t getCursor();
	screen_t getScreen();
private:
};

#endif /* DISPLAY_TEST_H_ */
