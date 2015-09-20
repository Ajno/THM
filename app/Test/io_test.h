/*
 * io_test.h
 *
 *  Created on: Jul 15, 2015
 *      Author: z0037ysj
 */

#ifndef IO_TEST_H_
#define IO_TEST_H_

#include "io.h"

void test_io_output_init(const io_t pin);
void test_io_output_run();

void test_io_inputOutput_init(const io_t cInPin, const io_t cOutPin);
void test_io_inputOutput_run();

#endif /* IO_TEST_H_ */
