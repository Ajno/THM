/*
 * base.h
 *
 *  Created on: Sep 20, 2015
 *      Author: Jano
 */

#ifndef BASE_H_
#define BASE_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*pAppCallback_t)(void);

extern void baseInitApp();
void baseInstallApp(pAppCallback_t const pCallback);

#ifdef __cplusplus
}
#endif
#endif /* BASE_H_ */

