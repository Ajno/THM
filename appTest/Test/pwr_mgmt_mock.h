/*
 * pwr_mgmt_mock.h
 *
 *  Created on: 7. 10. 2015
 *      Author: z0037ysj
 */

#ifndef PWR_MGMT_MOCK_H_
#define PWR_MGMT_MOCK_H_

class PwrMgmtMock
{
public:
    PwrMgmtMock();
    virtual ~PwrMgmtMock();
    bool isSleeping();
};

#endif /* PWR_MGMT_MOCK_H_ */
