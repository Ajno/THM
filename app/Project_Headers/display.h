/*
 * display.h
 *
 *  Created on: Dec 9, 2015
 *      Author: z0037ysj
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

void displayBacklightTurnOn();
void displayBacklightTurnOff();
void displayBacklightToggle();
Bool displayBacklightIsOn();
void displayInit();

#ifdef __cplusplus
}
#endif
#endif /* DISPLAY_H_ */
