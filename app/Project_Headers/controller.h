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
    cCursorPosEndOfLine1Menu2 = 30
};

typedef enum
{
	cMenuState_show1,
	cMenuState_show2,
	cMenuState_goto1,
	cMenuState_goto2,
	cMenuState_select2,
	cMenuState_unselect2,
	cMenuState_modify2
}menuState_t;

void controller();

#ifdef __cplusplus
}
#endif
#endif /* CONTROLLER_H_ */
