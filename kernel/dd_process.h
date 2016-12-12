/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_process.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 11:49      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_process_h
#define dd_process_h

class ddProcessPrivate;
class ddProcess {
	DD_PRIVATE_DECLARE(ddProcess)
public:
	ddProcess();
	~ddProcess();
	static ddBool createProcess(ddpCChar path, ddpCChar cmd[]);

	static ddBool getModuleName(ddpChar path, ddUInt16 len);
	static ddBool getModulePath(ddpChar path, ddUInt16 len);
};
#endif // dd_process_h

