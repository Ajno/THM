/*
 * base.cpp
 *
 *  Created on: 24.9.2015
 *      Author: Jano
 */
#include "base.h"

typedef void (*pAppCallback_t)(void);

extern void baseInitApp();
extern "C" void baseInstallApp(pAppCallback_t const pCallback)
{}



