/*
 * test.cpp
 *
 *  Created on: 24.9.2015
 *      Author: Jano
 */

#include "test.h"
#include "controller.h"
#include "display.h"
#include "base.h"
#include "base_mock.h"
#include "thm_lib.h"
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(ControllerTest);

const int cPositionMenu2 = 16;

void ControllerTest::setUp()
{
    pButtons = new ButtonsMock();
    pTimer = new TimerMock();
    pLcd = new LcdMock();
    pPwrMgmt = new PwrMgmtMock();
    pTemperature = new TemperatureMock(205);
    pOscillator = new OscillatorMock(150);
}

void ControllerTest::tearDown()
{
    delete pTimer;
    delete pLcd;
    delete pButtons;
    delete pPwrMgmt;
    delete pTemperature;
    delete pOscillator;
}

void ControllerTest::firstStart()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
}

void ControllerTest::firstTurnOnDisplay()
{
    ControllerTest::firstStart();
    controller();
}

void ControllerTest::shiftTheScreen()
{
    pButtons->setState(true, cButton_Lower);
    for (int i = 0; i < 100; ++i)
    {
        controller();
        pButtons->setState(false, cButton_Lower);
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
}

void ControllerTest::goSleep()
{
    for (uint i = 0; i < 2; ++i)
    {
        pTimer->stop(TimerMock::cTimerSec);
        controller();
    }
}

void ControllerTest::pressAndReleaseButton(const buttons_t cButton, const uint count)
{
    for (uint i = 0; i < count; ++i)
    {
        pButtons->setState(true, cButton);
        controller();
        pButtons->setState(false, cButton);
        controller();
    }
}

void ControllerTest::initApp()
{
    BaseMock* pBase = new BaseMock();

    baseInitApp();

    CPPUNIT_ASSERT(pBase->appInstalled());

    delete pBase;
}

void ControllerTest::backlightOn()
{
    baseInitApp();
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(!pLcd->lcdIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::backlightOff()
{
    baseInitApp();
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();

    CPPUNIT_ASSERT(!pLcd->backlightIsOn());
    CPPUNIT_ASSERT(!pLcd->lcdIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::backlightStayOn()
{
    ControllerTest::firstStart();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(!pLcd->lcdIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::turnOnDisplay()
{
    ControllerTest::firstTurnOnDisplay();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::turnOffBacklight()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();

    CPPUNIT_ASSERT(!pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::goToSleepInIdle()
{
    ControllerTest::firstTurnOnDisplay();
    controller();
    ControllerTest::goSleep();

    CPPUNIT_ASSERT(pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT(!pLcd->backlightIsOn());
    CPPUNIT_ASSERT(!pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::goToSleepInModifyAndWakeUp()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pressAndReleaseButton(cButton_Lower,1);
    goSleep();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(0, pLcd->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 35%           Vlhkost: 40%    " == pLcd->getScreen().text));
//    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_off, pLcd->getCursor().state);
}

void ControllerTest::wakeUpAfterMenu1()
{
    ControllerTest::firstStart();
    controller();
    ControllerTest::goSleep();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT_EQUAL(0, pLcd->getScreen().position);
}

void ControllerTest::wakeUpAfterMenu2()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    ControllerTest::shiftTheScreen();
    ControllerTest::goSleep();
    controller();
    pPwrMgmt->wakeUp();
    controller();
    pButtons->setState(true, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL(1, pLcd->getScreen().position);
}

void ControllerTest::shiftScreenRight()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(true, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(1, pLcd->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::slideToMenu2()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();// turn off backlight
    ControllerTest::shiftTheScreen();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::slideToMenu1()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pTimer->stop(TimerMock::cTimerSec);
    controller();// turn off backlight
    ControllerTest::shiftTheScreen();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(LcdMock::cCursor_off, pLcd->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(0, pLcd->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::selectMenu2Start()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_off, pLcd->getCursor().state);
}

void ControllerTest::selectMenu2End()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();// menu 2 selected
    pButtons->setState(true, cButton_Lower);
    controller();// stay at position 16
    pButtons->setState(false, cButton_Upper);
    pTimer->stop(TimerMock::cTimerMiliSecX100);// short timer elapsed, should have no effect
    controller();// stay at position 16

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_blinking, pLcd->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen), pLcd->getCursor().position);
}

void ControllerTest::increaseContrastBy5()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pressAndReleaseButton(cButton_Upper, 1);

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 45%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_blinking, pLcd->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 3), pLcd->getCursor().position);
}

void ControllerTest::increaseContrastToMax()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pressAndReleaseButton(cButton_Upper, 20);

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 100%          Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_blinking, pLcd->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 4), pLcd->getCursor().position);
}

void ControllerTest::decreaseContrastBy5()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pressAndReleaseButton(cButton_Lower, 1);

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 35%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_blinking, pLcd->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 3), pLcd->getCursor().position);
}

void ControllerTest::unselectMenu2Start()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pButtons->setState(true, cButton_Upper);
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_blinking, pLcd->getCursor().state);
}

void ControllerTest::unselectMenu2End()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_off, pLcd->getCursor().state);
}

void ControllerTest::decreaseContrastToMin()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pressAndReleaseButton(cButton_Lower, 20);

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pLcd->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 0%            Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_blinking, pLcd->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 2), pLcd->getCursor().position);
}

void ControllerTest::slideToMenu1AfterModify2()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pButtons->setState(true, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(false, cButton_Upper);
    controller();
    pButtons->setState(true, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pLcd->backlightIsOn());
    CPPUNIT_ASSERT(pLcd->lcdIsOn());
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == pLcd->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2 - 1, pLcd->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",LcdMock::cCursor_off, pLcd->getCursor().state);
}

void ControllerTest::itoaTest()
{
    string exp1 = "127";
    string act1(thmLibItoa(127));
    string exp2 = "1";
    string act2(thmLibItoa(1));
    string exp3 = "0";
    string act3(thmLibItoa(0));
    string exp4 = "25";
    string act4(thmLibItoa(25));
    string exp5 = "99";
    string act5(thmLibItoa(99));
    string exp6 = "-1";
    string act6(thmLibItoa(-1));
    string exp7 = "-25";
    string act7(thmLibItoa(-25));
    string exp8 = "-127";
    string act8(thmLibItoa(-127));
    string exp9 = "-32767";
    string act9(thmLibItoa(-32767));
    string exp10 = "32767";
    string act10(thmLibItoa(32767));

    CPPUNIT_ASSERT_EQUAL(exp1,act1);
    CPPUNIT_ASSERT_EQUAL(exp2,act2);
    CPPUNIT_ASSERT_EQUAL(exp3,act3);
    CPPUNIT_ASSERT_EQUAL(exp4,act4);
    CPPUNIT_ASSERT_EQUAL(exp5,act5);
    CPPUNIT_ASSERT_EQUAL(exp6,act6);
    CPPUNIT_ASSERT_EQUAL(exp7,act7);
    CPPUNIT_ASSERT_EQUAL(exp8,act8);
    CPPUNIT_ASSERT_EQUAL(exp9,act9);
    CPPUNIT_ASSERT_EQUAL(exp10,act10);
}

void ControllerTest::temperatureUpdate()
{
    const sWord cTemperature1 = 123;
    const sWord cTemperature2 = -151;
    string screenTemp1("");
    string screenTemp2("");

    ControllerTest::firstTurnOnDisplay();
    pTemperature->setTemperature(cTemperature1);
    // call controller until filter settle down
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenTemp1 = pLcd->getScreen().text;
    pTemperature->setTemperature(cTemperature2);
    // call controller until filter settle down
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenTemp2 = pLcd->getScreen().text;

    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSecX100));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 12,3ßC Kontrast: 40%           Vlhkost: 45%    " == screenTemp1));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: -15,1ßCKontrast: 40%           Vlhkost: 50%    " == screenTemp2));
}

void ControllerTest::humidityUpdate1()
{
    string screenHum1("");
    string screenHum2("");
    string screenHum3("");
    string screenHum4("");
    string screenHum5("");
    string screenHum6("");
    string screenHum7("");
    string screenHum8("");
    string screenHum9("");
    string screenHum10("");
    string screenHum11("");
    string screenHum12("");

    ControllerTest::firstTurnOnDisplay();
    // call controller until filter settle down
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum1 = pLcd->getScreen().text;
    pOscillator->setFrequency(19*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum2 = pLcd->getScreen().text;
    pOscillator->setFrequency(45670*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum3 = pLcd->getScreen().text;
    pOscillator->setFrequency(9549*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum4 = pLcd->getScreen().text;
    pOscillator->setFrequency(21008*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum5 = pLcd->getScreen().text;
    pOscillator->setFrequency(1750*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum6 = pLcd->getScreen().text;
    pOscillator->setFrequency(61789*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum7 = pLcd->getScreen().text;
    pOscillator->setFrequency(84*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum8 = pLcd->getScreen().text;
    pOscillator->setFrequency(70*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum9 = pLcd->getScreen().text;
    pOscillator->setFrequency(15000*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum10 = pLcd->getScreen().text;
    pOscillator->setFrequency(5000*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum11 = pLcd->getScreen().text;
    pOscillator->setFrequency(2500*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screenHum12 = pLcd->getScreen().text;

    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSecX100));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 40%    " == screenHum1));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 19%    " == screenHum2));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 89%    " == screenHum3));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 70%    " == screenHum4));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 80%    " == screenHum5));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 55%    " == screenHum6));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 94%    " == screenHum7));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 30%    " == screenHum8));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 27%    " == screenHum9));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 74%    " == screenHum10));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 62%    " == screenHum11));
    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 57%    " == screenHum12));
}

void ControllerTest::humidityUpdate2()
{
    string screen("");

    ControllerTest::firstTurnOnDisplay();
    pOscillator->setFrequency(1750*cSamplingPeriodMiliSecX100/10);
    pTimer->stop(TimerMock::cTimerMiliSecX100);
    controller();
    screen = pLcd->getScreen().text;

    CPPUNIT_ASSERT_MESSAGE(pLcd->getScreen().text,("Teplota: 20,5ßC Kontrast: 40%           Vlhkost: 55%    " == screen));
}

void ControllerTest::movAvgFilterTest()
{
    FILTER_BUFFER_T(4) buffer = FILTER_INIT(4);
    sWord input = 0;
    sWord output = 0;
    sWord expected = 0;

    expected = 10;
    input = -20;
    output = thmLibMovAvgFilter(input,buffer.data,buffer.len);
    output = thmLibMovAvgFilter(input,buffer.data,buffer.len);
    input = 40;
    output = thmLibMovAvgFilter(input,buffer.data,buffer.len);
    output = thmLibMovAvgFilter(input,buffer.data,buffer.len);

    CPPUNIT_ASSERT_EQUAL(expected,output);
}
