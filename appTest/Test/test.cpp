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
        controller();
        pTimer->stop(TimerMock::cTimerSec);
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::goToSleep()
{
    ControllerTest::firstTurnOnDisplay();
    ControllerTest::goSleep();
    controller();

    CPPUNIT_ASSERT(pPwrMgmt->isSleeping());
    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::wakeUpAfterMenu1()
{
    ControllerTest::firstStart();
    ControllerTest::goSleep();
    controller();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 80%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 100%         Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 70%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_blinking, pDisplay->getCursor().state);
}

void ControllerTest::unselectMenu2Stop()
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
    pButtons->setState(false, cButton_Upper);
    controller();
    pTimer->stop(TimerMock::cTimerMiliSec);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
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
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: 75%          Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(cPositionMenu2, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cursor state",DisplayMock::cCursor_off, pDisplay->getCursor().state);
}
