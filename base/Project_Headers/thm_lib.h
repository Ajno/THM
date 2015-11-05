/*
 * thm_lib.h
 *
 *  Created on: Nov 4, 2015
 *      Author: z0037ysj
 */

#ifndef THM_LIB_H_
#define THM_LIB_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdtypes.h>

/*
 * @brief integer to string
 *
 * @param[in] <-32767, 32767>
 */
char* thmLibItoa(const sWord cNum);

#ifdef __cplusplus
}
#endif
#endif /* THM_LIB_H_ */
