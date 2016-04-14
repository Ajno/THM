/*
 * controller.h
 *
 *  Created on: 2. 10. 2015
 *      Author: z0037ysj
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#ifdef __cplusplus
extern "C" {
#endif

enum staticConst_t
{ 
    cNumOfBacklightToggle = 7,
    cSamplingPeriodMiliSecX100 = 5 // 500 ms
};

typedef enum
{
    cState_sleep,
	cState_idle1,
	cState_idle2,
	cState_goto1,
	cState_goto2,
	cState_upperPressedInIdle2,
	cState_changeContrastActive,
	cState_upperPressedInChangeContrast,
	cState_waitToEnterIdle2,
	cState_lowerPressedInChangeContrast,
	cState_idleChangeContrast,
	cState_lowBatteryWarningOn,
	cState_lowBatteryWarningOff //todo
}menuState_t;

void controller();

#ifdef __cplusplus
}
#endif
#endif /* CONTROLLER_H_ */
