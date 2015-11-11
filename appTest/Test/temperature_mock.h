/*
 * temperature_mock.h
 *
 *  Created on: 5. 11. 2015
 *      Author: z0037ysj
 */

#ifndef TEMPERATURE_MOCK_H_
#define TEMPERATURE_MOCK_H_

#include <stdtypes.h>

class TemperatureMock
{
public:
    TemperatureMock(const sWord cTemperature);
    virtual ~TemperatureMock();
    void setTemperature(const sWord cTemperature);
};

#endif /* TEMPERATURE_MOCK_H_ */
