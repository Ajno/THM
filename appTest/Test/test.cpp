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
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::turnOnDisplay()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::turnOffBacklight()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    controller();
    pTimer->stop(TimerMock::cTimerSec);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::goToSleep()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    for (int i = 0; i < 2; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerSec);
    }
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::wakeUp()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    for (int i = 0; i < 2; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerSec);
    }
    controller();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
}

void ControllerTest::shiftScreenRight()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    controller();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(1, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerMiliSec));
}

void ControllerTest::slideToMenu2()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    controller();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    for (int i = 0; i < 100; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(16, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}

void ControllerTest::slideToMenu1()
{
    baseInitApp();
    for (int i = 0; i < (cNumOfBacklightToggle - 1); ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    controller();
    controller();
    pTimer->stop(TimerMock::cTimerSec);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    for (int i = 0; i < 100; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    for (int i = 0; i < 100; ++i)
    {
        controller();
        pTimer->stop(TimerMock::cTimerMiliSec);
    }

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(!pTimer->isRunning(TimerMock::cTimerMiliSec));
    CPPUNIT_ASSERT(pTimer->isRunning(TimerMock::cTimerSec));
}
