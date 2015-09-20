/*
 * base.h
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#ifndef BASE_H_
#define BASE_H_

typedef void (*pAppCallback_t)(void);

extern void baseInitApp();
void baseInstallApp(pAppCallback_t const pCallback);

#endif /* BASE_H_ */
