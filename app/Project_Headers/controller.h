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
    cMenuState_sleep,
	cMenuState_idle1,
	cMenuState_idle2,
	cMenuState_goto1,
	cMenuState_goto2,
	cMenuState_upperPressedInIdle2,
	cMenuState_waitToChangeContrast,
	cMenuState_upperPressedInChangeContrast,
	cMenuState_waitToEnterIdle2,
	cMenuState_lowerPressedInChangeContrast,
	cMenuState_changeContrast
}menuState_t;

void controller();

#ifdef __cplusplus
}
#endif
#endif /* CONTROLLER_H_ */
