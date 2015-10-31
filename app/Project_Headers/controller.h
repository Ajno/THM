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
    cContrastPositionOnScreen = 26
};

typedef enum
{
	cMenuState_display1,
	cMenuState_display2,
	cMenuState_goto1,
	cMenuState_goto2,
	cMenuState_upperPressedInDisplay2,
	cMenuState_waitBeforeSelect2,
	cMenuState_upperPressedInSelect2,
	cMenuState_waitBeforeDisplay2,
	cMenuState_pressLowerWhenSelect2,
	cMenuState_select2
}menuState_t;

void controller();

#ifdef __cplusplus
}
#endif
#endif /* CONTROLLER_H_ */
