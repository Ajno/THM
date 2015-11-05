/*
 * test.cpp
 *
 *  Created on: 24.9.2015
 *      Author: Jano
 */

#include "test.h"
#include "controller.h"
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
    pDisplay = new DisplayMock();
    pPwrMgmt = new PwrMgmtMock();
}

void ControllerTest::tearDown()
{
    delete pTimer;
    delete pDisplay;
    delete pButtons;
    delete pPwrMgmt;
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::backlightOff()
{
    baseInitApp();
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::backlightStayOn()
{
    ControllerTest::firstStart();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::turnOnDisplay()
{
    ControllerTest::firstTurnOnDisplay();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::turnOffBacklight()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::goToSleepInIdle()
{
    ControllerTest::firstTurnOnDisplay();
    controller();
    ControllerTest::goSleep();

    CPPUNIT_ASSERT(pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 45%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
//    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
}

void ControllerTest::wakeUpAfterMenu1()
{
    ControllerTest::firstStart();
    controller();
    ControllerTest::goSleep();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
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
    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL(1, pDisplay->getScreen().position);
}

void ControllerTest::shiftScreenRight()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(true, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(1, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::slideToMenu2()
{
    ControllerTest::firstTurnOnDisplay();
    pTimer->stop(TimerMock::cTimerSec);
    controller();// turn off backlight
    ControllerTest::shiftTheScreen();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::selectMenu2Start()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::shiftTheScreen();
    pButtons->setState(true, cButton_Upper);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
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
    controller();// stay at position 16

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen), pDisplay->getCursor().position);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 55%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 3), pDisplay->getCursor().position);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 100%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 4), pDisplay->getCursor().position);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 45%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 3), pDisplay->getCursor().position);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 0%            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", static_cast<int>(cContrastPositionOnScreen + 2), pDisplay->getCursor().position);
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

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 50%           Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2 - 1, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
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
