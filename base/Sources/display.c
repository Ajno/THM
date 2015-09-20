/*
 * display.c
 *
 *  Created on: Aug 6, 2015
 *      Author: z0037ysj
 */

#include "display.h"
#include "pwm.h"
#include "io.h"
#include "timer.h"

enum displayBus_t
{
    cDisplayBus_DB4         = cPin_B0,
    cDisplayBus_DB5         = cPin_B1,
    cDisplayBus_DB6         = cPin_B2,
    cDisplayBus_DB7         = cPin_B3,
    cDisplayBus_backLight   = cPin_B4,
    cDisplayBus_E           = cPin_B5,
    cDisplayBus_RW          = cPin_B6,
    cDisplayBus_RS          = cPin_B7
};

static const Byte cMask_Backligh = 0x10;

static Bool bDataBusConfiguredAsOutput = FALSE;

static void displayPrepareBusForWrite()
{
    Byte portB = 0;
    ioConfig_t pinCfg;
    
    if (!bDataBusConfiguredAsOutput)
    {
        pinCfg.bOutput = TRUE;
        pinCfg.bPullUp = FALSE;
        ioConfigure(cDisplayBus_DB4,pinCfg);
        ioConfigure(cDisplayBus_DB5,pinCfg);
        ioConfigure(cDisplayBus_DB6,pinCfg);
        ioConfigure(cDisplayBus_DB7,pinCfg);
        bDataBusConfiguredAsOutput = pinCfg.bOutput;
    }
    // clear port B except backlight
    ioReadPortB(&portB);
    portB = portB & cMask_Backligh;
    ioWritePortB(portB);
}

static void displayToggleEnable()
{
    // set enable
    ioWrite(cDisplayBus_E,TRUE);
    // wait
    wait500ns();
    // clear enable
    ioWrite(cDisplayBus_E,FALSE);
    // wait
    wait500ns();
}

static void displayReadBusyAndAddress(Bool* pBusy, Byte* pAddress)
{
    ioConfig_t pinCfg;
    Byte upperBits = 0;
    Byte lowerBits = 0;
    
    if (bDataBusConfiguredAsOutput)
    {
        // configure data bits as inputs
        pinCfg.bOutput = FALSE;
        pinCfg.bPullUp = FALSE;
        ioConfigure(cDisplayBus_DB4,pinCfg);
        ioConfigure(cDisplayBus_DB5,pinCfg);
        ioConfigure(cDisplayBus_DB6,pinCfg);
        ioConfigure(cDisplayBus_DB7,pinCfg);
        bDataBusConfiguredAsOutput = pinCfg.bOutput;
    }
    // clear RS
    ioWrite(cDisplayBus_RS,FALSE);
    // set RW
    ioWrite(cDisplayBus_RW,TRUE);
    wait500ns();
    // set enable
    ioWrite(cDisplayBus_E,TRUE);
    wait500ns();
    // read upper 4 bits
    ioRead(cDisplayBus_DB7, pBusy);
    ioReadPortB(&upperBits);
    // clear enable
    ioWrite(cDisplayBus_E,TRUE);
    wait500ns();
    
    // set enable
    ioWrite(cDisplayBus_E,TRUE);
    wait500ns();
    // read lower 4 bits
    ioReadPortB(&lowerBits); 
    // clear enable
    ioWrite(cDisplayBus_E,TRUE);
    wait500ns();
    
    upperBits = ((upperBits << 4) & 0x70);
    *pAddress = (lowerBits & 0x0F) | (upperBits & 0xF0);
}

static void displayWaitTillNotBusy()
{
    Bool bBusy = TRUE;
    Byte address = 0;
    
    do
    {
        displayReadBusyAndAddress(&bBusy, &address);
    } while (bBusy);
}

void displayClear()
{
    displayPrepareBusForWrite();
    displayToggleEnable();
    
    // set bit 0
    ioWrite(cDisplayBus_DB4, TRUE);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

void displayReturnHome()
{
    displayPrepareBusForWrite();
    displayToggleEnable();
    
    // set bit 1
    ioWrite(cDisplayBus_DB5, TRUE);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

void displayOnOffControl(const displayOnOffControl_t cControl)
{
    displayPrepareBusForWrite();
    displayToggleEnable();
    
    /* 
     * write lower 4 bits
     * set bit 3 in X
     * set bit D in X
     * set bit C in X
     * set bit B in X
     * */
    ioWrite(cDisplayBus_DB7,TRUE);
    ioWrite(cDisplayBus_DB6,cControl.bDisplayOn);
    ioWrite(cDisplayBus_DB5,cControl.bCursorOn);
    ioWrite(cDisplayBus_DB4,cControl.bBlinkingCursorOn);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

void displayOrCursorShift(const displayMovingDirection_t cSetting)
{
    displayPrepareBusForWrite();
    ioWrite(cDisplayBus_DB4, TRUE);
    displayToggleEnable();
    
    // write lower 4 bits, set S/C, set R/L
    ioWrite(cDisplayBus_DB7, cSetting.bShiftScreenInsteadOfCursor);
    ioWrite(cDisplayBus_DB6, cSetting.bShiftRightInsteadOfLeft);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

static void displayFunctionSet()
{
    displayPrepareBusForWrite();
    // set bit 1 of higher byte
    ioWrite(cDisplayBus_DB5,TRUE);
    displayToggleEnable();
    
    // write lower 4 bits, set bit N in X,  bit F has no meaning when N is set
    ioWrite(cDisplayBus_DB7,TRUE);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

void displayEntryModeSet(const displayMovingDirection_t cSetting)
{
    displayPrepareBusForWrite();
    // set bit 2 in higher 4 bits
    ioWrite(cDisplayBus_DB6,TRUE);
    displayToggleEnable();
    
    // write lower 4 bits, set bit ID, set bit S
    ioWrite(cDisplayBus_DB5, cSetting.bShiftRightInsteadOfLeft);
    ioWrite(cDisplayBus_DB4,cSetting.bShiftScreenInsteadOfCursor);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

static void displayWriteDDRAM(const Byte cData)
{
    Byte portB = 0;
    
    displayPrepareBusForWrite();
    // set RS
    ioWrite(cDisplayBus_RS, TRUE);
    // write upper 4 bits
    ioReadPortB(&portB);
    portB = portB | ((cData >> 4) & 0x0F);
    ioWritePortB(portB);
    displayToggleEnable();
    
    // write lower 4 bits
    portB = portB & 0xF0;
    portB = portB | (cData & 0x0F);
    ioWritePortB(portB);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

void displayMoveCursor(const Byte cAddress)
{
    Byte portB = 0;
    
    displayPrepareBusForWrite();
    // set bit 4 in higher 4 bits
    ioWrite(cDisplayBus_DB7,TRUE);
    // write upper 4 bits
    ioReadPortB(&portB);
    portB |= ((cAddress >> 4) & 0x07);
    ioWritePortB(portB);
    displayToggleEnable();
    
    // write lower 4 bits
    portB = portB & 0xF0;
    portB |= (cAddress & 0x0F);
    ioWritePortB(portB);
    displayToggleEnable();
    
    displayWaitTillNotBusy();
}

void displayWrite(const uchar* pString)
{
	Byte i;
	
	for (i = 0; pString[i] != 0; ++i) 
	{
		displayWriteDDRAM(pString[i]);
		displayWaitTillNotBusy();
	}
}

void displayBackLightOn(const Bool bBackLightOn)
{
    ioWrite(cDisplayBus_backLight, bBackLightOn);
}

static void displayFirstStart()
{
    displayOnOffControl_t onOffSetting;
    displayMovingDirection_t dirSetting;
    
    // wait 45 ms
    waitX100us(450);
        
    // write DB5 DB4
    displayPrepareBusForWrite();
    ioWrite(cDisplayBus_DB5, TRUE);
    ioWrite(cDisplayBus_DB4, TRUE);
    displayToggleEnable();
    
    // wait 5 ms
    waitX100us(50);
    
    // write DB5 DB4
    displayToggleEnable();
    
    // wait 0,5 ms
    waitX100us(5);
    
    // write DB5 DB4
    displayToggleEnable();
    
    //Function set (Set interface to be 4 bits long.)
    //Interface is 8 bits in length.
    // write upper 4 bits
    ioWrite(cDisplayBus_DB4, FALSE);
    displayToggleEnable();
    displayWaitTillNotBusy();
    
    //Function set (Interface is 4 bits long. Specify the
    //number of display lines and character font.)
    //The number of display lines and character font
    //cannot be changed after this point.
    displayFunctionSet();
    
    //Display off
    onOffSetting.bDisplayOn = FALSE;
    onOffSetting.bCursorOn = FALSE;
    onOffSetting.bBlinkingCursorOn = FALSE;
    displayOnOffControl(onOffSetting);
    
    //Display clear
    displayClear();
    
    //Entry mode set
    dirSetting.bShiftRightInsteadOfLeft = TRUE;
    dirSetting.bShiftScreenInsteadOfCursor = FALSE;
    displayEntryModeSet(dirSetting);
}

void displayInit()
{
    ioConfig_t pinCfg;
    pwmChannelConfig_t chnlCfg;
    const Word cChannelValue = (pwmReadModulo() / 100) * 5 - 3;// todo

    pinCfg.bOutput = TRUE;
    ioConfigurePortB(pinCfg);
    bDataBusConfiguredAsOutput = TRUE;

    chnlCfg.mode = cPwmMode_edgeAligned_clear;
    pwmConfigureChannel(chnlCfg);
    pwmWriteChannel(cChannelValue);
    
    displayFirstStart();
}
