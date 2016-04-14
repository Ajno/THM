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

static const Word cBacklightToggleTimeMiliSec = 250;
static const Word cAwakeTimeSec = 10;
static const Word cButtonPressTimeMiliSec = 2000;

static Byte cntrBacklightToggle = cNumOfBacklightToggle;
static menuState_t state = cState_idle1;
static menuState_t stateBeforeLowBatWarning = cState_idle1;

FILTER_BUFFER_T(4) temperatureSamples = FILTER_INIT(4);
static sWord temperatureRaw = 0;
static sWord temperatureFilt = 0;
static Word humidityRaw = 0;

void updateTemperatureAndHumidity()
{
    temperatureRaw = temperatureRead();
    temperatureFilt = thmLibMovAvgFilter(temperatureRaw,
            temperatureSamples.data, temperatureSamples.len);
    humidityRaw = humidityRead(temperatureRaw, 10 / cSamplingPeriodMiliSecX100);
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
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
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
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
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
        // fixme
        displayMenuTemplate();
        displayContrastSet();
        state = stateBeforeLowBatWarning;
    }
    updateTemperatureAndHumidity();
    displayDoAnimation();
    timerRestartMiliSecX100(cSamplingPeriodMiliSecX100);
}

void onElapsedLongTimer()
{
    if (displayBacklightIsOn())
    {
        displayBacklightTurnOff();
        timerRestartSec(cAwakeTimeSec);
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
        timerRestartSec(cAwakeTimeSec);

        switch (state)
        {
        case cState_idle2:
            timerRestartMiliSec(cButtonPressTimeMiliSec);
            state = cState_upperPressedInIdle2;
            break;
        case cState_idleChangeContrast:
            timerRestartMiliSec(cButtonPressTimeMiliSec);
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
        timerRestartSec(cAwakeTimeSec);

        switch (state)
        {
        case cState_idle1:
            displaySlideRight();
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
            state = cState_goto2;
            break;
        case cState_idle2:
            displaySlideLeft();
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
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

void handleLowBattery()
{
    if (pwgMgmtIsLowBattery())
    {
        switch (state)
        {
            case cState_idle1:
            case cState_idle2:
            case cState_idleChangeContrast:
                stateBeforeLowBatWarning = state;
                state = cState_lowBatteryWarningOn;
                displayLowBatteryWarning();
                timerRestartMiliSecX100(cSamplingPeriodMiliSecX100);
                break;
            default:
                break;
        }
    }
    else
    {
        if (cState_lowBatteryWarningOn == state)
        {
            // fixme
            displayMenuTemplate();
            displayContrastSet();
            state = stateBeforeLowBatWarning;
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
                timerRestartMiliSec(cBacklightToggleTimeMiliSec);
            }
        }
    }
    else
    {
        if (!displayIsOn())
        {
            displayTurnOn();
            displayBacklightTurnOn();
            updateTemperatureAndHumidity();
            displayContrastSet();
            timerRestartSec(cAwakeTimeSec);
        }
        else
        {
            handleLowBattery();
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
