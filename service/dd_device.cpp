/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_device.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-08 11:07      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include "../dandan.h"
ddDevice::ddDevice()
{
}

ddDevice::~ddDevice()
{
}
	
emDeviceId ddDevice::deviceId()
{
	return DDENUM_DEVICEID_START;
}

ddBool ddDevice::isMyCode(ddUInt32 iocode)
{
	return no;
}

ddVoid ddDevice::onIoctl(ddUInt32 iocode, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout)
{
}

ddVoid ddDevice::onMessage(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam)
{
}

ddVoid ddDevice::send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId)
{
	DD_GLOBAL_INSTANCE_DO(ddDevManager, send(msg, wParam, lParam, deviceId));
}

ddVoid ddDevice::broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam)
{
	DD_GLOBAL_INSTANCE_DO(ddDevManager, broadcast(msg, wParam, lParam));
}

