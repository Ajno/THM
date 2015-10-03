/*
 * base.cpp
 *
 *  Created on: 24.9.2015
 *      Author: Jano
 */
#include "base.h"
#include "base_mock.h"

static bool bInstalled = false;

extern "C" void baseInstallApp(pAppCallback_t const pCallback)
{
    if (0 != pCallback)
    {
        bInstalled = true;
    }
}

BaseMock::BaseMock()
{
    bInstalled = false;
}

bool BaseMock::appInstalled()
{
    return bInstalled;
}
