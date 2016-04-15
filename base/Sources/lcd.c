/*
 * display.c
 *
 *  Created on: Aug 6, 2015
 *      Author: z0037ysj
 */

#include "lcd.h"
#include "pwm.h"
#include "io.h"
#include "timer.h"

enum displayBus_t
{
    cLcdBus_DB4         = cPin_B0,
    cLcdBus_DB5         = cPin_B1,
    cLcdBus_DB6         = cPin_B2,
    cLcdBus_DB7         = cPin_B3,
    cLcdBus_backLight   = cPin_B4,
    cLcdBus_E           = cPin_B5,
    cLcdBus_RW          = cPin_B6,
    cLcdBus_RS          = cPin_B7
};

static const Word cDutyCycle = 45;// 4,5% duty cycle
static const Byte cMask_Backligh = 0x10;
static const Word cContrastToDutyCycleOffset = 28;
static const Word cDutyCycleToContrastQuotient = 5;
static Bool bDataBusConfiguredAsOutput = FALSE;

static void lcdPrepareBusForWrite()
{
    Byte portB = 0;
    ioConfig_t pinCfg;
    
    // clear port B except backlight
    ioReadPortB(&portB);
    portB = portB & cMask_Backligh;
    ioWritePortB(portB);
    if (!bDataBusConfiguredAsOutput)
    {
        pinCfg.bOutput = TRUE;
        pinCfg.bPullUp = FALSE;
        ioConfigure(cLcdBus_DB4,pinCfg);
        ioConfigure(cLcdBus_DB5,pinCfg);
        ioConfigure(cLcdBus_DB6,pinCfg);
        ioConfigure(cLcdBus_DB7,pinCfg);
        bDataBusConfiguredAsOutput = pinCfg.bOutput;
    }
}

static void lcdToggleEnable()
{
    // set enable
    ioWrite(cLcdBus_E,TRUE);
    // wait
    wait500ns();
    // clear enable
    ioWrite(cLcdBus_E,FALSE);
    // wait
    wait500ns();
}

static void lcdReadBusyAndAddress(Bool* pBusy, Byte* pAddress)
{
    ioConfig_t pinCfg;
    Byte upperBits = 0;
    Byte lowerBits = 0;
    
    if (bDataBusConfiguredAsOutput)
    {
        // configure data bits as inputs
        pinCfg.bOutput = FALSE;
        pinCfg.bPullUp = FALSE;
        ioConfigure(cLcdBus_DB4,pinCfg);
        ioConfigure(cLcdBus_DB5,pinCfg);
        ioConfigure(cLcdBus_DB6,pinCfg);
        ioConfigure(cLcdBus_DB7,pinCfg);
        bDataBusConfiguredAsOutput = pinCfg.bOutput;
    }
    // clear RS
    ioWrite(cLcdBus_RS,FALSE);
    // set RW
    ioWrite(cLcdBus_RW,TRUE);
    wait500ns();
    // set enable
    ioWrite(cLcdBus_E,TRUE);
    wait500ns();
    // read upper 4 bits
    ioRead(cLcdBus_DB7, pBusy);
    ioReadPortB(&upperBits);
    // clear enable
    ioWrite(cLcdBus_E,FALSE);
    wait500ns();
    
    // set enable
    ioWrite(cLcdBus_E,TRUE);
    wait500ns();
    // read lower 4 bits
    ioReadPortB(&lowerBits); 
    // clear enable
    ioWrite(cLcdBus_E,FALSE);
    wait500ns();
    
    upperBits = ((upperBits << 4) & 0x70);
    *pAddress = (lowerBits & 0x0F) | (upperBits & 0xF0);
}

static void lcdWaitTillNotBusy()
{
    Bool bBusy = TRUE;
    Byte address = 0;
    
    do
    {
        lcdReadBusyAndAddress(&bBusy, &address);
    } while (bBusy);
}

void lcdClear()
{
    lcdPrepareBusForWrite();
    lcdToggleEnable();
    
    // set bit 0
    ioWrite(cLcdBus_DB4, TRUE);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

void lcdReturnHome()
{
    lcdPrepareBusForWrite();
    lcdToggleEnable();
    
    // set bit 1
    ioWrite(cLcdBus_DB5, TRUE);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

void lcdOnOffControl(const lcdOnOffControl_t cControl)
{
    lcdPrepareBusForWrite();
    lcdToggleEnable();
    
    /* 
     * write lower 4 bits
     * set bit 3 in X
     * set bit D in X
     * set bit C in X
     * set bit B in X
     * */
    ioWrite(cLcdBus_DB7,TRUE);
    ioWrite(cLcdBus_DB6,cControl.bLcdOn);
    ioWrite(cLcdBus_DB5,cControl.bCursorOn);
    ioWrite(cLcdBus_DB4,cControl.bBlinkingCursor);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

void lcdScreenOrCursorShift(const lcdMovingDirection_t cSetting)
{
    lcdPrepareBusForWrite();
    ioWrite(cLcdBus_DB4, TRUE);
    lcdToggleEnable();
    
    // write lower 4 bits, set S/C, set R/L
    ioWrite(cLcdBus_DB7, cSetting.bShiftScreenInsteadOfCursor);
    ioWrite(cLcdBus_DB6, cSetting.bShiftRightInsteadOfLeft);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

static void lcdFunctionSet()
{
    lcdPrepareBusForWrite();
    // set bit 1 of higher byte
    ioWrite(cLcdBus_DB5,TRUE);
    lcdToggleEnable();
    
    // write lower 4 bits, set bit N in X,  bit F has no meaning when N is set
    ioWrite(cLcdBus_DB7,TRUE);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

void lcdEntryModeSet(const lcdMovingDirection_t cSetting)
{
    lcdPrepareBusForWrite();
    // set bit 2 in higher 4 bits
    ioWrite(cLcdBus_DB6,TRUE);
    lcdToggleEnable();
    
    // write lower 4 bits, set bit ID, set bit S
    ioWrite(cLcdBus_DB5, cSetting.bShiftRightInsteadOfLeft);
    ioWrite(cLcdBus_DB4,cSetting.bShiftScreenInsteadOfCursor);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

static void lcdWriteDDRAM(const Byte cData)
{
    Byte portB = 0;
    
    lcdPrepareBusForWrite();
    // set RS
    ioWrite(cLcdBus_RS, TRUE);
    // write upper 4 bits
    ioReadPortB(&portB);
    portB = portB | ((cData >> 4) & 0x0F);
    ioWritePortB(portB);
    lcdToggleEnable();
    
    // write lower 4 bits
    portB = portB & 0xF0;
    portB = portB | (cData & 0x0F);
    ioWritePortB(portB);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

void lcdMoveCursor(const Byte cAddress)
{
    Byte portB = 0;
    
    lcdPrepareBusForWrite();
    // set bit 4 in higher 4 bits
    ioWrite(cLcdBus_DB7,TRUE);
    // write upper 4 bits
    ioReadPortB(&portB);
    portB |= ((cAddress >> 4) & 0x07);
    ioWritePortB(portB);
    lcdToggleEnable();
    
    // write lower 4 bits
    portB = portB & 0xF0;
    portB |= (cAddress & 0x0F);
    ioWritePortB(portB);
    lcdToggleEnable();
    
    lcdWaitTillNotBusy();
}

void lcdWrite(const char* pString)
{
	Byte i;
	
	for (i = 0; pString[i] != 0; ++i) 
	{
		lcdWriteDDRAM(pString[i]);
		lcdWaitTillNotBusy();
	}
}

void lcdSetContrast(const Word cContrast)
{
    Word dutyCycle = cContrast;
    if (cLcdMaxContrast < cContrast) 
    {                
        dutyCycle = cLcdMaxContrast;
    }
    
    dutyCycle = cContrastToDutyCycleOffset + (dutyCycle / cDutyCycleToContrastQuotient);
    pwmWriteChannel(dutyCycle);
    
}
Word lcdGetContrast()
{
    Word ret = pwmReadChannel();
    ret = (ret - cContrastToDutyCycleOffset) * cDutyCycleToContrastQuotient;
    return ret;
}

void lcdBackLightOn(const Bool bBackLightOn)
{
    ioWrite(cLcdBus_backLight, bBackLightOn);
}

static void lcdFirstStart()
{
    lcdOnOffControl_t onOffSetting;
    lcdMovingDirection_t dirSetting;
    
    // wait 45 ms
    waitX100us(450);
        
    // write DB5 DB4
    lcdPrepareBusForWrite();
    ioWrite(cLcdBus_DB5, TRUE);
    ioWrite(cLcdBus_DB4, TRUE);
    lcdToggleEnable();
    
    // wait 5 ms
    waitX100us(50);
    
    // write DB5 DB4
    lcdToggleEnable();
    
    // wait 0,5 ms
    waitX100us(5);
    
    // write DB5 DB4
    lcdToggleEnable();
    
    //Function set (Set interface to be 4 bits long.)
    //Interface is 8 bits in length.
    // write upper 4 bits
    ioWrite(cLcdBus_DB4, FALSE);
    lcdToggleEnable();
    lcdWaitTillNotBusy();
    
    //Function set (Interface is 4 bits long. Specify the
    //number of display lines and character font.)
    //The number of display lines and character font
    //cannot be changed after this point.
    lcdFunctionSet();
    
    //Display off
    onOffSetting.bLcdOn = FALSE;
    onOffSetting.bCursorOn = FALSE;
    onOffSetting.bBlinkingCursor = FALSE;
    lcdOnOffControl(onOffSetting);
    
    //Display clear
    lcdClear();
    
    //Entry mode set
    dirSetting.bShiftRightInsteadOfLeft = TRUE;
    dirSetting.bShiftScreenInsteadOfCursor = FALSE;
    lcdEntryModeSet(dirSetting);
}

void lcdPrepareForSleep()
{
    ioConfig_t pinCfg;
    
    lcdWaitTillNotBusy();
    
    // these pins have external pullup 
    ioWrite(cLcdBus_RS, TRUE);
    ioWrite(cLcdBus_RW, TRUE);
    
    pinCfg.bOutput = FALSE;
    pinCfg.bPullUp = FALSE;
    ioConfigure(cLcdBus_DB4,pinCfg);
    ioConfigure(cLcdBus_DB5,pinCfg);
    ioConfigure(cLcdBus_DB6,pinCfg);
    ioConfigure(cLcdBus_DB7,pinCfg);
    bDataBusConfiguredAsOutput = FALSE;
}

void lcdInit()
{
    ioConfig_t pinCfg;
    pwmChannelConfig_t chnlCfg;
    const Word cChannelValue = (pwmReadModulo() * cDutyCycle) / 1000;

    pinCfg.bOutput = TRUE;
    pinCfg.bPullUp = FALSE;
    ioConfigurePortB(pinCfg);
    bDataBusConfiguredAsOutput = TRUE;

    chnlCfg.mode = cPwmMode_edgeAligned_clear;
    pwmConfigureChannel(chnlCfg);
    pwmWriteChannel(cChannelValue);
    
    lcdFirstStart();
}
