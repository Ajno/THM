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

class MyFirstTest : public TestFixture
{
    CPPUNIT_TEST_SUITE(MyFirstTest);

    CPPUNIT_TEST(testFirstCall);
    CPPUNIT_TEST(testInitApp);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testInitApp();
	void testFirstCall();
};

#endif /* TEST_H_ */
