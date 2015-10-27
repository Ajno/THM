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

void ControllerTest::callUntilInitEnd()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
}

void ControllerTest::callUntilTurnOnDisplayAfterInit()
{
    ControllerTest::callUntilInitEnd();
    controller();
}

void ControllerTest::callUntilShiftTheScreen()
{
    for (int i = 0; i < 100; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
}

void ControllerTest::callUntilSleep()
{
    for (int i = 0; i < 2; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerSec);
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
    ControllerTest::callUntilInitEnd();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::turnOnDisplay()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::turnOffBacklight()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pTimer->stop(TimerMock::cTimerSec);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::goToSleep()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    ControllerTest::callUntilSleep();
    controller();

    CPPUNIT_ASSERT(pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::wakeUpAfterMenu1()
{
    ControllerTest::callUntilInitEnd();
    ControllerTest::callUntilSleep();
    controller();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
}

void ControllerTest::wakeUpAfterMenu2()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();
    ControllerTest::callUntilSleep();
    controller();
    pPwrMgmt->wakeUp();
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL(1, pDisplay->getScreen().position);
}

void ControllerTest::shiftScreenRight()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(1, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::slideToMenu2()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pTimer->stop(TimerMock::cTimerSec);
    controller();// turn off backlight
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::slideToMenu1()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();
    pTimer->stop(TimerMock::cTimerSec);
    controller();// turn off backlight
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::selectMenu2()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();
    pButtons->setState(cButtonState_JustPressed, cButton_Upper);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
}

void ControllerTest::modifyMenu2()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();
    pButtons->setState(cButtonState_JustPressed, cButton_Upper);
    controller();
    pButtons->setState(cButtonState_Pressed, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    controller();// stay at position 16

    const int cCursorPosition = pDisplay->getScreen().text.length();
    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor position", cCursorPosition, pDisplay->getCursor().position);
}

void ControllerTest::unselectMenu2()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();
    pButtons->setState(cButtonState_JustPressed, cButton_Upper);
    controller();
    pButtons->setState(cButtonState_Pressed, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Upper);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
}

void ControllerTest::showModifiedMenu2()
{
    ControllerTest::callUntilTurnOnDisplayAfterInit();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    ControllerTest::callUntilShiftTheScreen();
    pButtons->setState(cButtonState_JustPressed, cButton_Upper);
    controller();
    pButtons->setState(cButtonState_Pressed, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Upper);
    controller();
    pButtons->setState(cButtonState_Pressed, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast:  75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
}
