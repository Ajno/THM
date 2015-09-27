/*
 * test.cpp
 *
 *  Created on: 24.9.2015
 *      Author: Jano
 */

#include "cppunit/TestCase.h"
using namespace CppUnit;

class myFirstTest : public TestCase
{
public:

void runTest()
{
	 CPPUNIT_ASSERT( true );
}

private:
};

