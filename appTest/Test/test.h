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
#include "lcd_mock.h"
#include "timer_mock.h"
#include "pwr_mgmt_mock.h"
#include "temperature_mock.h"
#include "oscillator_mock.h"

using namespace CppUnit;

class ControllerTest: public TestFixture
{
    ButtonsMock* pButtons;
    TimerMock* pTimer;
    LcdMock* pLcd;
    PwrMgmtMock* pPwrMgmt;
    TemperatureMock* pTemperature;
    OscillatorMock* pOscillator;

CPPUNIT_TEST_SUITE(ControllerTest);

    CPPUNIT_TEST(initApp);
    CPPUNIT_TEST(backlightOn);
    CPPUNIT_TEST(backlightOff);
    CPPUNIT_TEST(backlightStayOn);
    CPPUNIT_TEST(turnOnDisplay);
    CPPUNIT_TEST(turnOffBacklight);
    CPPUNIT_TEST(goToSleepInIdle);
    CPPUNIT_TEST(goToSleepInModifyAndWakeUp);
    CPPUNIT_TEST(wakeUpAfterMenu1);
    CPPUNIT_TEST(shiftScreenRight);
    CPPUNIT_TEST(slideToMenu2);
    CPPUNIT_TEST(slideToMenu1);
    CPPUNIT_TEST(wakeUpAfterMenu2);
    CPPUNIT_TEST(selectMenu2Start);
    CPPUNIT_TEST(selectMenu2End);
    CPPUNIT_TEST(increaseContrastBy5);
    CPPUNIT_TEST(increaseContrastToMax);
    CPPUNIT_TEST(decreaseContrastBy5);
    CPPUNIT_TEST(decreaseContrastToMin);
    CPPUNIT_TEST(unselectMenu2Start);
    CPPUNIT_TEST(unselectMenu2End);
    CPPUNIT_TEST(slideToMenu1AfterModify2);
    CPPUNIT_TEST(itoaTest);
    CPPUNIT_TEST(movAvgFilterTest);
    CPPUNIT_TEST(temperatureUpdate);
    CPPUNIT_TEST(humidityUpdate1);
    CPPUNIT_TEST(humidityUpdate2);
    CPPUNIT_TEST(lowBatteryWarning);

    CPPUNIT_TEST_SUITE_END()
    ;
public:
    void setUp();
    void tearDown();

    void firstStart();
    void firstTurnOnDisplay();
    void shiftTheScreen();
    void goSleep();
    void pressAndReleaseButton(const buttons_t cButton, const uint count);

    void initApp();
    void backlightOn();
    void backlightOff();
    void backlightStayOn();
    void turnOnDisplay();
    void turnOffBacklight();
    void goToSleepInIdle();
    void goToSleepInModifyAndWakeUp();
    void wakeUpAfterMenu1();
    void wakeUpAfterMenu2();
    void shiftScreenRight();
    void slideToMenu2();
    void slideToMenu1();
    void selectMenu2Start();
    void selectMenu2End();
    void increaseContrastBy5();
    void increaseContrastToMax();
    void decreaseContrastBy5();
    void decreaseContrastToMin();
    void unselectMenu2Start();
    void unselectMenu2End();
    void slideToMenu1AfterModify2();
    void itoaTest();
    void movAvgFilterTest();
    void temperatureUpdate();
    void humidityUpdate1();
    void humidityUpdate2();
    void lowBatteryWarning();
};

#endif /* TEST_H_ */
