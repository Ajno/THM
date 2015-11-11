/*
 * temperature_mock.cpp
 *
 *  Created on: 5. 11. 2015
 *      Author: z0037ysj
 */

#include <temperature_mock.h>
#include <temperature.h>

static sWord temperature = 0;

extern "C" sWord temperatureRead()
{
    return temperature;
}

TemperatureMock::TemperatureMock(const sWord cTemperature)
{
    temperature = cTemperature;
}

TemperatureMock::~TemperatureMock()
{
    // TODO Auto-generated destructor stub
}

void TemperatureMock::setTemperature(const sWord cTemperature)
{
    temperature = cTemperature;
}
