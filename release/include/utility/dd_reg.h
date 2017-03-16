/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_reg.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-01-03 15:02      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_reg_h
#define dd_reg_h

class ddRegPrivate;
class ddReg {
	DD_PRIVATE_DECLARE(ddReg)
public:
	ddReg(ddpCChar pPattern);///< 规则支持最大256个字符
	~ddReg();
	ddBool comp();
    ddBool exec(ddpCChar data);
    ddVoid error(ddpChar buf, ddUInt16 size);
    ddVoid free();
};
#endif // dd_reg_h

