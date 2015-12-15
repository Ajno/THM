/*
 * oscillator_mock.h
 *
 *  Created on: 15. 12. 2015
 *      Author: z0037ysj
 */

#ifndef OSCILLATOR_MOCK_H_
#define OSCILLATOR_MOCK_H_

#include <stdtypes.h>

class OscillatorMock
{
public:
    OscillatorMock(const Word cFreq);
    virtual ~OscillatorMock();
};

#endif /* OSCILLATOR_MOCK_H_ */
