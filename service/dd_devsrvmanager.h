/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_devsrvmanager.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 19:39      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_devsrvmanager_h
#define dd_devsrvmanager_h

class ddDevManagerPrivate;
class ddDevManager {
	DD_PRIVATE_DECLARE(ddDevManager)
public:
	ddDevManager();
	~ddDevManager();
	ddVoid add(ddDevice *pDevice);
	ddVoid remove(ddDevice *pDevice);
	ddVoid ioctl(ddUInt16 iocmd, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout);

	ddVoid send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId);
	ddVoid broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam);
};

class ddSrvManagerPrivate;
class ddSrvManager {
	DD_PRIVATE_DECLARE(ddSrvManager)
public:
	class listener {
	public:
		listener();
		virtual ~listener() {}
		virtual ddUInt16 myCommand() = 0;
		virtual ddVoid onProtocol(ddCommand& cmd) = 0;
	};

	class notifier {
	public:
		notifier();
		virtual ~notifier() {}
		virtual ddBool isMyCommand(ddUInt16 command) = 0;
		virtual ddVoid onProtocol(ddCommand& cmd) = 0;
	};
	ddSrvManager();
	~ddSrvManager();
	ddVoid add(listener* pListener);
	ddVoid remove(listener *pListener);
	ddVoid add(notifier* pNotifier);
	ddVoid remove(notifier *pNotifier);
	ddVoid notify(ddpCByte data, ddUInt16 len);
};

#endif // dd_devsrvmanager_h
