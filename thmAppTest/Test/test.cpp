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

void ControllerTest::testInitApp()
{
    BaseMock* pBase = new BaseMock();

    baseInitApp();

    CPPUNIT_ASSERT(pBase->appInstalled());
}

void ControllerTest::testFirstWakeUp1()
{
	DisplayMock* pDisplay = new DisplayMock();
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();

    CPPUNIT_ASSERT(pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::testFirstWakeUp2()
{
	DisplayMock* pDisplay = new DisplayMock();
	TimerMock*	pTimer = new TimerMock();

	baseInitApp();
	controller();
    pTimer->stop();
    controller();

    CPPUNIT_ASSERT(!pDisplay->backlightIsOn());
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::testFirstWakeUp3()
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
    CPPUNIT_ASSERT(pTimer->isRunning());
}

void ControllerTest::testFirstWakeUp4()
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
    CPPUNIT_ASSERT(!pTimer->isRunning());
}
