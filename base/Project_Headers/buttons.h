/*
 * buttons.h
 *
 *  Created on: Aug 6, 2015
 *      Author: z0037ysj
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

typedef enum
{
    cButton_Upper = 0,
    cButton_Lower = 1
}buttons_t;

void buttonsInit();
Bool buttonIsPressed(const buttons_t cButton);

#ifdef __cplusplus
}
#endif
#endif /* BUTTONS_H_ */
