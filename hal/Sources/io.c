/*
 * io_pins.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: z0037ysj
 */

#include "io.h"
#include <derivative.h>

static const Byte cByteMax = 0xFF;

static const Byte cIoIdx2IoMask[cPin_B7 + 1] = 
{
    PTAD_PTAD1_MASK,
    PTAD_PTAD2_MASK,
    PTAD_PTAD3_MASK,
    PTBD_PTBD0_MASK,
    PTBD_PTBD1_MASK,
    PTBD_PTBD2_MASK,
    PTBD_PTBD3_MASK,
    PTBD_PTBD4_MASK,
    PTBD_PTBD5_MASK,
    PTBD_PTBD6_MASK,
    PTBD_PTBD7_MASK
};

void ioConfigure(const io_t idx, const ioConfig_t config)
{
    if (cPin_A3 < idx) 
    {        
        if (config.bOutput) 
        {            
            PTBDD |= cIoIdx2IoMask[idx];
        } 
        else 
        {
            PTBDD &= (~cIoIdx2IoMask[idx]);
        }
        
        if (config.bPullUp) 
        {            
            PTBPE |= cIoIdx2IoMask[idx];
        } 
        else 
        {
            PTBPE &= (~cIoIdx2IoMask[idx]);
        }
    } 
    else 
    {
        if (config.bOutput) 
        {            
            PTADD |= cIoIdx2IoMask[idx];
        } 
        else 
        {
            PTADD &= (~cIoIdx2IoMask[idx]);
        }
        
        if (config.bPullUp) 
        {            
            PTAPE |= cIoIdx2IoMask[idx];
        } 
        else 
        {
            PTAPE &= (~cIoIdx2IoMask[idx]);
        }      
    }
}

void ioConfigurePortB(const ioConfig_t config)
{
    if (config.bOutput)
    {
        PTBDD = cByteMax;        
    }
    else
    {
        PTBDD = 0;        
    }
    
    if (config.bPullUp)
    {
        PTBPE = cByteMax;
    }
    else
    {
        PTBPE = 0;        
    }
}

void ioWrite(const io_t idx, const Bool bValue)
{
    if (cPin_A3 < idx) 
    {        
        if (bValue) 
        {            
            PTBD |= cIoIdx2IoMask[idx];
        } 
        else 
        {
            PTBD &= (~cIoIdx2IoMask[idx]);
        }
    } 
    else 
    {
        if (bValue) 
        {            
            PTAD |= cIoIdx2IoMask[idx];
        } 
        else 
        {
            PTAD &= (~cIoIdx2IoMask[idx]);
        }      
    }
}

void ioWritePortB(const Byte cValue)
{
    PTBD = cValue;
}

void ioRead(const io_t idx, Bool* pValue)
{
	Word value = 0;
	
    if (cPin_A3 < idx) 
    {        
        value = PTBD;        
    } 
    else 
    {
        value = PTAD; 
    }
    
    value &= (cIoIdx2IoMask[idx]);
    *pValue = (0 < value); 
}

void ioReadPortB(Byte* pValue)
{
    *pValue = PTBD;
}
