/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_devmanager.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-08 11:09      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include "../dandan.h"

class ddDevManagerPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddDevManager)
public:
	ddDevManagerPrivate() {
	}

	~ddDevManagerPrivate() {
	}
	
	ddVoid add(ddDevice *pDevice) {
		if ( pDevice ) {
			ddBool isExist = no;
			std::list<ddDevice *>::const_iterator it = m_listDevice.begin();
			while ( it != m_listDevice.end() ) {
				if ( pDevice == *it ) {
					isExist = yes;
					break;
				}
				it++;
			}
			if ( !isExist ) {
				m_listDevice.push_back(pDevice);
			}
		}
	}

	ddVoid remove(ddDevice *pDevice) {
		if ( pDevice ) {
			m_listDevice.remove(pDevice);
		}
	}

	ddVoid clear() {
		m_listDevice.clear();
	}

	ddVoid ioctl(ddUInt32 iocode, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout) {
		std::list<ddDevice *>::const_iterator it = m_listDevice.begin();
		while ( it != m_listDevice.end() ) {
			if ( (*it)->isMyCode(iocode) ) {
				(*it)->onIoctl(iocode, pin, uin, pout, uout);
				break;
			}
			it++;
		}
	}

	ddVoid send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId) {
		std::list<ddDevice *>::const_iterator it = m_listDevice.begin();
		while ( it != m_listDevice.end() ) {
			if ( deviceId == (*it)->deviceId() ) {
				(*it)->onMessage(msg, wParam, lParam);
				break;
			}
			it++;
		}
	}

	ddVoid broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam) {
		std::list<ddDevice *>::const_iterator it = m_listDevice.begin();
		while ( it != m_listDevice.end() ) {
			(*it)->onMessage(msg, wParam, lParam);
			it++;
		}
	}
private:
	std::list<ddDevice *> m_listDevice;
};

///////////////////////////////////////////////////////////////////////////////
ddDevManager::ddDevManager()
{
	DD_D_NEW(ddDevManagerPrivate);
}

ddDevManager::~ddDevManager()
{
	DD_D_DELETE();
}

ddVoid ddDevManager::add(ddDevice *pDevice)
{
	dPtr()->add(pDevice);
}

ddVoid ddDevManager::remove(ddDevice *pDevice)
{
	dPtr()->remove(pDevice);
}

ddVoid ddDevManager::clear()
{
	dPtr()->clear();
}

ddVoid ddDevManager::ioctl(ddUInt32 iocode, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout)
{
	printf("ddDevManager,ioctl,iocode:%d,uin:%d,uout:%d\n", iocode, uin, uout);
	dPtr()->ioctl(iocode, pin, uin, pout, uout);
}

ddVoid ddDevManager::send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId)
{
	dPtr()->send(msg, wParam, lParam, deviceId);
}

ddVoid ddDevManager::broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam)
{
	dPtr()->broadcast(msg, wParam, lParam);
}

