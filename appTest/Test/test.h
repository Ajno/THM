/*
 * test.h
 *
 *  Created on: 27.9.2015
 *      Author: Jano
 */

#ifndef TEST_H_
#define TEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "buttons_mock.h"
#include "display_mock.h"
#include "timer_mock.h"
#include "pwr_mgmt_mock.h"
using namespace CppUnit;

class ControllerTest : public TestFixture
{
    ButtonsMock* pButtons;
    TimerMock* pTimer;
    DisplayMock* pDisplay;
    PwrMgmtMock* pPwrMgmt;

    CPPUNIT_TEST_SUITE(ControllerTest);

    CPPUNIT_TEST(initApp);
    CPPUNIT_TEST(backlightOn);
    CPPUNIT_TEST(backlightOff);
    CPPUNIT_TEST(backlightStayOn);
    CPPUNIT_TEST(turnOnDisplay);
    CPPUNIT_TEST(turnOffBacklight);
    CPPUNIT_TEST(goToSleep);
    CPPUNIT_TEST(wakeUpAfterMenu1);
    CPPUNIT_TEST(shiftScreenRight);
    CPPUNIT_TEST(slideToMenu2);
    CPPUNIT_TEST(slideToMenu1);
    CPPUNIT_TEST(wakeUpAfterMenu2);
    CPPUNIT_TEST(selectMenu2Start);
    CPPUNIT_TEST(selectMenu2End);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void init();

    void initApp();
	void backlightOn();
	void backlightOff();
	void backlightStayOn();
	void turnOnDisplay();
	void turnOffBacklight();
	void goToSleep();
	void wakeUpAfterMenu1();
	void wakeUpAfterMenu2();
	void shiftScreenRight();
	void slideToMenu2();
	void slideToMenu1();
	void selectMenu2Start();
	void selectMenu2End();
};

#endif /* TEST_H_ */
