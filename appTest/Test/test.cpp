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
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::backlightOff()
{
    baseInitApp();
    callControllerAndStopTimer(1, *pTimer);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::backlightStayOn()
{
    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning());
}

void ControllerTest::turnOnDisplay()
{
    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::turnOffBacklight()
{
    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(1, *pTimer);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::goToSleep()
{
    baseInitApp();

    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(2, *pTimer);
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(!pTimer->isRunning());
    CPPUNIT_ASSERT(pPwrMgmt->isSleeping());
}

void ControllerTest::wakeUp()
{
    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(2, *pTimer);
    controller();
    pPwrMgmt->wakeUp();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT(pTimer->isRunning());
    CPPUNIT_ASSERT(!pPwrMgmt->isSleeping());
}

void ControllerTest::shiftScreenRight()
{
//    baseInitApp();
//    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
//    controller();
//    callControllerAndStopTimer(1, *pTimer);
//    controller();
//    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
//    controller();
//
//    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
//    CPPUNIT_ASSERT(pDisplay->displayIsOn());
//    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
//    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
//    CPPUNIT_ASSERT_EQUAL(1, pDisplay->getScreen().position);
//    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::slideToMenu2()
{
    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(1, *pTimer);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(16, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::slideToMenu1()
{
    baseInitApp();
    callControllerAndStopTimer((cToggleBacklight - 1), *pTimer);
    controller();
    callControllerAndStopTimer(1, *pTimer);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    callControllerAndStopTimer(1, *pTimer);
    controller();
    pButtons->setState(cButtonState_JustPressed, cButton_Lower);
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursor().state);
    CPPUNIT_ASSERT_MESSAGE(pDisplay->getScreen().text,("Teplota neznama Kontrast: --            Vlhkost neznama Jazyk: SVK" == pDisplay->getScreen().text));
    CPPUNIT_ASSERT_EQUAL(0, pDisplay->getScreen().position);
    CPPUNIT_ASSERT(pTimer->isRunning());
}
