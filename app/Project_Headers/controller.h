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
    cNumOfBacklightToggle = 7 
};

typedef enum
{
	cScreenState_menu1,
	cScreenState_menu2,
	cScreenState_gotoMenu1,
	cScreenState_gotoMenu2
}screenState_t;

void controller();

#ifdef __cplusplus
}
#endif
#endif /* CONTROLLER_H_ */
