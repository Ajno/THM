/*
 * io_test.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: z0037ysj
 */

#include "io_test.h"

static struct
{
	Bool 	val;
	io_t 	idx;
	Word 	cntr;
}test_out;

void test_io_output_init(const io_t cPin)
{
	ioConfig_t outputCfg;
	outputCfg.bOutput = TRUE;
	test_out.val = FALSE;
	
	ioConfigure(cPin, outputCfg);

	test_out.idx = cPin;
	test_out.cntr = 0;
}

void test_io_output_run()
{
	const Word toogle = 20000;
	test_out.cntr++;
	if (toogle < test_out.cntr)
	{			
		(FALSE == test_out.val) ? (test_out.val = TRUE):(test_out.val = FALSE);
		ioWrite(test_out.idx,test_out.val);
		test_out.cntr = 0;
	}
}

static struct 
{
	io_t	inIdx;
	io_t	outIdx;
}test_inOut;

void test_io_inputOutput_init(const io_t cInPin, const io_t cOutPin)
{
	ioConfig_t inputCfg;
	ioConfig_t outputCfg;
	
	inputCfg.bOutput = FALSE;
	inputCfg.bPullUp = TRUE;	
	outputCfg.bOutput = TRUE;
	
	test_inOut.inIdx = cInPin;
	test_inOut.outIdx = cOutPin;
	
	ioConfigure(test_inOut.inIdx, inputCfg);
	ioConfigure(test_inOut.outIdx, outputCfg);
}

void test_io_inputOutput_run()
{
	auto Bool value = FALSE;
	ioRead(test_inOut.inIdx,&value);
	ioWrite(test_inOut.outIdx,value);
}
