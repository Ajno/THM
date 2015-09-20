/*
 * system.h
 *
 *  Created on: Sep 19, 2015
 *      Author: Jano
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdtypes.h>

typedef struct
{
    Bool bStopModeEn;
    Bool bLowVoltageDetectionEn;
    Bool bLowVoltageDetectionInStopModeEn;
    Bool bLowVoltageResetEn;
    Bool bLowVoltageTripPointHigh;
    Bool bLowVoltageWarningPointHigh;
}systemConfig_t;

typedef struct
{
    Bool bLowVoltageDetect;
    Bool bIllegalAddress;
    Bool bIllegalOpcode;
    Bool bCOPWatchdog;
    Bool bExternalResetPin;
    Bool bPowerOnReset;
}systemRecentResetSource_t;

void systemConfigure(const systemConfig_t cCfg);
void systemResetWatchdog();
void systemEnableInterrupts();
void systemWait();
void systemStop();
Bool systemLowVoltageWarningDetected();
void systemClearLowVoltageWarning();
void systemReadResetSource(systemRecentResetSource_t* pSource);

#endif /* SYSTEM_H_ */
