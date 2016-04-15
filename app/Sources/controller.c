/*
 * app.c
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */
#include "thm_lib.h"
#include "base.h"
#include "buttons.h"
#include "timer.h"
#include "display.h"
#include "pwr_mgmt.h"
#include "temperature.h"
#include "humidity.h"
#include "controller.h"

static const Word cTimeoutBacklightToggle = 250; // 250 ms
static const Word cTimeoutAwake = 10; // 10 sec
static const Word cTimeoutHoldButton = 2000; // 2000 ms

static Byte cntrBacklightToggle = cNumOfBacklightToggle;
static menuState_t state = cState_idle1;
static menuState_t stateBeforeLowBatWarning = cState_idle1;

FILTER_BUFFER_T(4)
temperatureSamples = FILTER_INIT(4);
static sWord temperatureRaw = 0;
static sWord temperatureFilt = 0;
static Word humidityRaw = 0;

void updateAndDisplayTempAndHum()
{
    temperatureRaw = temperatureRead();
    temperatureFilt = thmLibMovAvgFilter(temperatureRaw,
            temperatureSamples.data, temperatureSamples.len);
    humidityRaw = humidityRead(temperatureRaw, 10 / cTimeoutSamplingPeriod);
    displayTemperatureSet(temperatureFilt);
    displayHumiditySet(humidityRaw);
}

void onElapsedVeryShortTimer()
{
    switch (state)
    {
    case cState_goto1:
        if (displayIsNotSliding())
        {
            state = cState_idle1;
        }
        else
        {
            displaySlideLeft();
            timerRestartMiliSec(cTimeoutScreenShift);
        }
        break;
    case cState_goto2:
        if (displayIsNotSliding())
        {
            state = cState_idle2;
        }
        else
        {
            displaySlideRight();
            timerRestartMiliSec(cTimeoutScreenShift);
        }
        break;
    case cState_upperPressedInIdle2:
        displayCursorTurnOn();
        state = cState_changeContrastActive;
        break;
    case cState_upperPressedInChangeContrast:
        displayCursorTurnOff();
        state = cState_waitToEnterIdle2;
        break;
    default:
        break;
    }
}

void onElapsedShortTimer()
{
    if (cState_lowBatteryWarningOn == state)
    {
        state = stateBeforeLowBatWarning;
        displayMenuTemplate();
        displayContrastSet();
    }
    else
    {
        if (pwrMgmtIsLowBattery())
        {
            stateBeforeLowBatWarning = state;
            state = cState_lowBatteryWarningOn;
            displayLowBatteryWarning();
        }
    }

    if (cState_lowBatteryWarningOn != state)
    {
        updateAndDisplayTempAndHum();
        displayDoAnimation();
    }

    timerRestartMiliSecX100(cTimeoutSamplingPeriod);
}

void onElapsedLongTimer()
{
    if (displayBacklightIsOn())
    {
        displayBacklightTurnOff();
        timerRestartSec(cTimeoutAwake);
    }
    else
    {
        displayTurnOff();
        state = cState_sleep;
        pwrMgmtGoToSleep(TRUE);
    }
}

void handleUpperButton()
{
    if (buttonIsPressed(cButton_Upper))
    {
        displayBacklightTurnOn();
        timerRestartSec(cTimeoutAwake);

        switch (state)
        {
        case cState_idle2:
            timerRestartMiliSec(cTimeoutHoldButton);
            state = cState_upperPressedInIdle2;
            break;
        case cState_idleChangeContrast:
            timerRestartMiliSec(cTimeoutHoldButton);
            state = cState_upperPressedInChangeContrast;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (state)
        {
        case cState_upperPressedInIdle2:
            state = cState_idle2;
            break;
        case cState_changeContrastActive:
            state = cState_idleChangeContrast;
            break;
        case cState_upperPressedInChangeContrast:
            displayContrastIncrement();
            state = cState_idleChangeContrast;
            break;
        case cState_waitToEnterIdle2:
            state = cState_idle2;
            break;
        default:
            break;
        }
    }
}

void handleLowerButton()
{
    if (buttonIsPressed(cButton_Lower))
    {
        displayBacklightTurnOn();
        timerRestartSec(cTimeoutAwake);

        switch (state)
        {
        case cState_idle1:
            displaySlideRight();
            timerRestartMiliSec(cTimeoutScreenShift);
            state = cState_goto2;
            break;
        case cState_idle2:
            displaySlideLeft();
            timerRestartMiliSec(cTimeoutScreenShift);
            state = cState_goto1;
            break;
        case cState_idleChangeContrast:
            state = cState_lowerPressedInChangeContrast;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (state)
        {
        case cState_lowerPressedInChangeContrast:
            displayContrastDecrement();
            state = cState_idleChangeContrast;
            break;
        case cState_sleep:
            state = cState_idle1;
            break;
        default:
            break;
        }
    }
}

void controller()
{
    if (0 < cntrBacklightToggle)
    {
        if (timerElapsedMiliSec())
        {
            displayBacklightToggle();
            cntrBacklightToggle--;
            if (0 != cntrBacklightToggle)
            {
                timerRestartMiliSec(cTimeoutBacklightToggle);
            }
        }
    }
    else
    {
        if (!displayIsOn())
        {
            displayTurnOn();
            displayBacklightTurnOn();
            updateAndDisplayTempAndHum();
            displayContrastSet();
            timerRestartSec(cTimeoutAwake);
        }
        else
        {
            handleLowerButton();
            handleUpperButton();
        }

        if (timerElapsedMiliSec())
        {
            onElapsedVeryShortTimer();
        }

        if (timerElapsedMiliSecX100())
        {
            onElapsedShortTimer();
        }

        if (timerElapsedSec())
        {
            onElapsedLongTimer();
        }
    }
}

void baseInitApp()
{
    cntrBacklightToggle = cNumOfBacklightToggle;
    state = cState_idle1;
    stateBeforeLowBatWarning = cState_idle1;
    displayInit();
    baseInstallApp(&controller);
}
