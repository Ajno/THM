/*
 * test.h
 *
 *  Created on: 27.9.2015
 *      Author: Jano
 */

#ifndef TEST_H_
#define TEST_H_

#include <cppunit/extensions/HelperMacros.h>
using namespace CppUnit;

class ControllerTest : public TestFixture
{
    CPPUNIT_TEST_SUITE(ControllerTest);

    CPPUNIT_TEST(testInitApp);
    CPPUNIT_TEST(testFirstWakeUp1);
    CPPUNIT_TEST(testFirstWakeUp2);
    CPPUNIT_TEST(testFirstWakeUp3);
    CPPUNIT_TEST(testFirstWakeUp4);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testInitApp();
	void testFirstWakeUp1();
	void testFirstWakeUp2();
	void testFirstWakeUp3();
	void testFirstWakeUp4();
};

#endif /* TEST_H_ */
