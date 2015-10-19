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

    CPPUNIT_TEST(initApp);
    CPPUNIT_TEST(firstWakeUp1);
    CPPUNIT_TEST(firstWakeUp2);
    CPPUNIT_TEST(firstWakeUp3);
    CPPUNIT_TEST(firstWakeUp4);
    CPPUNIT_TEST(turnOnDisplay);
    CPPUNIT_TEST(turnOffBacklight);
    CPPUNIT_TEST(goToSleep);
    CPPUNIT_TEST(wakeUp);
    CPPUNIT_TEST(slideScreen);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void initApp();
	void firstWakeUp1();
	void firstWakeUp2();
	void firstWakeUp3();
	void firstWakeUp4();
	void turnOnDisplay();
	void turnOffBacklight();
	void goToSleep();
	void wakeUp();
	void slideScreen();
};

#endif /* TEST_H_ */
