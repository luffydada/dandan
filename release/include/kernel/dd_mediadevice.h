/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_mediadevice.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-14 13:51      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_mediadevice_h
#define dd_mediadevice_h

typedef enum {
	DDENUM_MEDIADEVICE_UNKNOW,
	DDENUM_MEDIADEVICE_SDCARD,
	DDENUM_MEDIADEVICE_USB,
	DDENUM_MEDIADEVICE_IPOD
}emMediaDeviceType, *pemMediaDeviceType;

class ddMediaDevicePrivate;
class ddMediaDevice {
	DD_PRIVATE_DECLARE(ddMediaDevice)
public:
	class interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onMediadevice_attached(emMediaDeviceType deviceType) {}
		virtual ddVoid onMediadevice_detached(emMediaDeviceType deviceType) {}
	};
	ddMediaDevice(interface *pOwner);
	~ddMediaDevice();
	ddVoid setOwner(interface *pOwner);
	ddBool start();
};

#endif // dd_mediadevice_h

