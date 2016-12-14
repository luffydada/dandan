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

class ddMediaDevicePrivate;
class ddMediaDevice {
	DD_PRIVATE_DECLARE(ddMediaDevice)
public:
	class interface {
		virtual ~interface();
	};
	ddMediaDevice();
	~ddMediaDevice();
	ddBool start();
};

#endif // dd_mediadevice_h

