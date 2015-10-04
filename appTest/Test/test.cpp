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
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(ControllerTest);

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
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::firstWakeUp2()
{
	DisplayMock* pDisplay = new DisplayMock();
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();
    pTimer->stop();
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::firstWakeUp3()
{
	DisplayMock* pDisplay = new DisplayMock();
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();
    pTimer->stop();
    controller();
    pTimer->stop();
    controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::firstWakeUp4()
{
	DisplayMock* pDisplay = new DisplayMock();
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();
    pTimer->stop();
    controller();
    pTimer->stop();
    controller();
    pTimer->stop();
    controller();
    pTimer->stop();
	controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(!pDisplay->displayIsOn());
    CPPUNIT_ASSERT(!pTimer->isRunning());
}

void ControllerTest::turnOnDisplay()
{
	DisplayMock* pDisplay = new DisplayMock();
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();
    pTimer->stop();
    controller();
    pTimer->stop();
    controller();
    pTimer->stop();
    controller();
    pTimer->stop();
	controller();

	controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pDisplay->displayIsOn());
    CPPUNIT_ASSERT_EQUAL(DisplayMock::cCursor_off, pDisplay->getCursorState());
    CPPUNIT_ASSERT(pTimer->isRunning());
}
