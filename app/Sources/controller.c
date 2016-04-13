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
static menuState_t menu = cMenuState_idle1;

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
    switch (menu)
    {
    case cMenuState_goto1:
        if (displayIsNotSliding())
        {
            menu = cMenuState_idle1;
        }
        else
        {
            displaySlideLeft();
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
        }
        break;
    case cMenuState_goto2:
        if (displayIsNotSliding())
        {
            menu = cMenuState_idle2;
        }
        else
        {
            displaySlideRight();
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
        }
        break;
    case cMenuState_upperPressedInIdle2:
        displayCursorTurnOn();
        menu = cMenuState_waitToChangeContrast;
        break;
    case cMenuState_upperPressedInChangeContrast:
        displayCursorTurnOff();
        menu = cMenuState_waitToEnterIdle2;
        break;
    default:
        break;
    }
}

void onElapsedShortTimer()
{
    if (pwgMgmtIsLowBattery())
    {
        displayLowBatteryWarning();
    }
    else
    {
        updateTemperatureAndHumidity();
        displayDoAnimation();
    }
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
        menu = cMenuState_sleep;
        pwrMgmtGoToSleep(TRUE);
    }
}

void processUpperButton()
{
    if (buttonIsPressed(cButton_Upper))
    {
        displayBacklightTurnOn();
        timerRestartSec(cAwakeTimeSec);

        switch (menu)
        {
        case cMenuState_idle2:
            timerRestartMiliSec(cButtonPressTimeMiliSec);
            menu = cMenuState_upperPressedInIdle2;
            break;
        case cMenuState_changeContrast:
            timerRestartMiliSec(cButtonPressTimeMiliSec);
            menu = cMenuState_upperPressedInChangeContrast;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (menu)
        {
        case cMenuState_upperPressedInIdle2:
            menu = cMenuState_idle2;
            break;
        case cMenuState_waitToChangeContrast:
            menu = cMenuState_changeContrast;
            break;
        case cMenuState_upperPressedInChangeContrast:
            displayContrastIncrement();
            menu = cMenuState_changeContrast;
            break;
        case cMenuState_waitToEnterIdle2:
            menu = cMenuState_idle2;
            break;
        default:
            break;
        }
    }
}

void processLowerButton()
{
    if (buttonIsPressed(cButton_Lower))
    {
        displayBacklightTurnOn();
        timerRestartSec(cAwakeTimeSec);

        switch (menu)
        {
        case cMenuState_idle1:
            displaySlideRight();
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
            menu = cMenuState_goto2;
            break;
        case cMenuState_idle2:
            displaySlideLeft();
            timerRestartMiliSec(cScreenShiftTimeMiliSec);
            menu = cMenuState_goto1;
            break;
        case cMenuState_changeContrast:
            menu = cMenuState_lowerPressedInChangeContrast;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (menu)
        {
        case cMenuState_lowerPressedInChangeContrast:
            displayContrastDecrement();
            menu = cMenuState_changeContrast;
            break;
        case cMenuState_sleep:
            menu = cMenuState_idle1;
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
            processLowerButton();
            processUpperButton();
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
    menu = cMenuState_idle1;
    displayInit();
    baseInstallApp(&controller);
}
