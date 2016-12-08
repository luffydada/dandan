/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_devmanager.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-08 11:08      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_devmanager_h
#define dd_devmanager_h

class ddDevManagerPrivate;
class ddDevManager {
	DD_PRIVATE_DECLARE(ddDevManager)
public:
	ddDevManager();
	~ddDevManager();
	ddVoid add(ddDevice *pDevice);
	ddVoid remove(ddDevice *pDevice);
	ddVoid clear();
	ddVoid ioctl(ddUInt32 iocode, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout);

	ddVoid send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId);
	ddVoid broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam);
};
#endif // dd_devmanager_h

