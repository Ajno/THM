/*
 * test.cpp
 *
 *  Created on: 24.9.2015
 *      Author: Jano
 */

#include "test.h"
#include "controller.h"
#include "base.h"
#include "base_test.h"
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(MyFirstTest);

void MyFirstTest::setUp()
{

}

void MyFirstTest::tearDown()
{

}

void MyFirstTest::testInitApp()
{
    BaseTest* pBase = new BaseTest();
    baseInitApp();
    CPPUNIT_ASSERT(pBase->getFlag());
}

void MyFirstTest::testFirstCall()
{
    cyclic();
    CPPUNIT_ASSERT(false);
}

