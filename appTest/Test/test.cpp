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
#include "display_mock.h"
#include "timer_mock.h"
#include "pwr_mgmt_mock.h"
#include "buttons_mock.h"
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(ControllerTest);

void callControllerAndStopTimer(const int cCount, TimerMock& rTimer)
{
    for (int i = 0; i < cCount; ++i)
    {

        controller();
        rTimer.stop();
    }
}

void ControllerTest::setUp()
{

}

void ControllerTest::tearDown()
{

}

void ControllerTest::initApp()
{
    BaseMock* pBase = new BaseMock();

    baseInitApp();

    CPPUNIT_ASSERT(pBase->appInstalled());
}

void ControllerTest::firstWakeUp1()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();

    baseInitApp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::firstWakeUp2()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();

    baseInitApp();
    callControllerAndStopTimer(1, *pTimer);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::firstWakeUp3()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();

    baseInitApp();
    callControllerAndStopTimer(2, *pTimer);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::firstWakeUp4()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();

    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning());
}

void ControllerTest::turnOnDisplay()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();

    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursorState());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen(),("Teplota neznama Kontrast: --            Vlhkost neznama  Jazyk: SK" == pDisplay->getScreen()));
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::turnOffBacklight()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();

    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(1, *pTimer);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursorState());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen(),("Teplota neznama Kontrast: --            Vlhkost neznama  Jazyk: SK" == pDisplay->getScreen()));
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::goToSleep()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();
    PwrMgmtMock* pPwrMgmt = new PwrMgmtMock();

    baseInitApp();

    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(2, *pTimer);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursorState());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen(),("Teplota neznama Kontrast: --            Vlhkost neznama  Jazyk: SK" == pDisplay->getScreen()));
    CPPUNIT_ASSERT(!pTimer->isRunning());
    CPPUNIT_ASSERT(pPwrMgmt->isSleeping());
}

void ControllerTest::wakeUp()
{
    DisplayMock* pDisplay = new DisplayMock();
    TimerMock* pTimer = new TimerMock();
    PwrMgmtMock* pPwrMgmt = new PwrMgmtMock();

    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(2, *pTimer);
    controller();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursorState());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen(),("Teplota neznama Kontrast: --            Vlhkost neznama  Jazyk: SK" == pDisplay->getScreen()));
    CPPUNIT_ASSERT(pTimer->isRunning());
    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
}

void ControllerTest::slideScreen()
{
    ButtonsMock* pButtons = new ButtonsMock();
    TimerMock* pTimer = new TimerMock();
    DisplayMock* pDisplay = new DisplayMock();

    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    controller();
    // todo push button
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursorState());
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen(),("Teplota neznama Kontrast: --            Vlhkost neznama  Jazyk: SK" == pDisplay->getScreen()));
    CPPUNIT_ASSERT_EQUAL(41, pDisplay->getCursorPosition());
}
