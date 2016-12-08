/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_device.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-08 11:08      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_device_h
#define dd_device_h

typedef enum {
	DDENUM_DEVICEID_START = 0,
	DDENUM_DEVICEID_RADIO,
	DDENUM_DEVICEID_BLUETOOTH,
	DDENUM_DEVICEID_MEDIA,
	DDENUM_DEVICEID_END
} emDeviceId;

#define DDDEF_IOCODE_RADIO			100
#define DDDEF_IOCODE_BLUETOOTH		200
#define DDDEF_IOCODE_MEDIA			300

class ddDevice {
public:
	ddDevice();
	~ddDevice();
	virtual emDeviceId deviceId();
	virtual ddBool isMyCode(ddUInt32 iocode);
	virtual ddVoid onIoctl(ddUInt32 iocode, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout);
	virtual ddVoid onMessage(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam);

	ddVoid send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId);
	ddVoid broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam);
};
#endif // dd_device_h

