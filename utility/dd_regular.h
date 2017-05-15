/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_regular.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-05-15 14:03      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_regular_h
#define dd_regular_h

class ddRegularPrivate;
class ddRegular {
	DD_PRIVATE_DECLARE(ddRegular)
public:
	ddRegular(ddpCChar pPattern);///< 规则支持最大256个字符
	~ddRegular();
	ddBool compile();
    ddBool exec(ddpCChar data);
    ddVoid error(ddpChar buf, ddUInt16 size);
    ddVoid free();
};
#endif // dd_regular_h
