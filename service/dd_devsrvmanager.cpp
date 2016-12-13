/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_devsrvmanager.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 19:39      V1.0.0                build         *
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

	ddVoid ioctl(ddUInt16 iocmd, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout) {
		if ( DDDEF_IOCOMMAND_PROTOCOL == iocmd ) {
			ddCommand cmd(reinterpret_cast<ddpCByte>(pin), uin);
			std::list<ddDevice *>::const_iterator it = m_listDevice.begin();
			while ( it != m_listDevice.end() ) {
				if ( (*it)->isMyProtocol(cmd.command()) ) {
					(*it)->onProtocol(cmd);
					break;
				}
				it++;
			}
			return;
		}
		std::list<ddDevice *>::const_iterator it = m_listDevice.begin();
		while ( it != m_listDevice.end() ) {
			if ( (*it)->isMyCommand(iocmd) ) {
				(*it)->onIoctl(iocmd, pin, uin, pout, uout);
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

ddVoid ddDevManager::ioctl(ddUInt16 iocmd, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout)
{
	dPtr()->ioctl(iocmd, pin, uin, pout, uout);
}

ddVoid ddDevManager::send(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam, emDeviceId deviceId)
{
	dPtr()->send(msg, wParam, lParam, deviceId);
}

ddVoid ddDevManager::broadcast(ddUInt32 msg, ddUInt32 wParam, ddUInt32 lParam)
{
	dPtr()->broadcast(msg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
class ddSrvManagerPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddSrvManager)
public:
	ddSrvManagerPrivate() {
	}

	~ddSrvManagerPrivate() {
	}
	
	ddVoid add(ddSrvManager::listener* pListener) {
		if ( pListener ) {
			ddBool isExist = no;
			std::list<ddSrvManager::listener *>::const_iterator it = m_listListener.begin();
			while ( it != m_listListener.end() ) {
				if ( pListener == *it ) {
					isExist = yes;
					break;
				}
				it++;
			}
			if ( !isExist ) {
				m_listListener.push_back(pListener);
			}
		}
	}

	ddVoid remove(ddSrvManager::listener *pListener) {
		if ( pListener ) {
			m_listListener.remove(pListener);
		}
	}

	ddVoid add(ddSrvManager::notifier* pNotifier) {
		if ( pNotifier ) {
			ddBool isExist = no;
			std::list<ddSrvManager::notifier *>::const_iterator it = m_listNotifier.begin();
			while ( it != m_listNotifier.end() ) {
				if ( pNotifier == *it ) {
					isExist = yes;
					break;
				}
				it++;
			}
			if ( !isExist ) {
				m_listNotifier.push_back(pNotifier);
			}
		}
	}

	ddVoid remove(ddSrvManager::notifier *pNotifier) {
		if ( pNotifier ) {
			m_listNotifier.remove(pNotifier);
		}
	}

	ddVoid notify(ddpCByte data, ddUInt16 len) {
		if ( data && len ) {
			ddCommand cmd(data, len);
			if ( DDENUM_COMMAND_SERVICE == cmd.type() ) {
				std::list<ddSrvManager::listener *>::const_iterator it = m_listListener.begin();
				while ( it != m_listListener.end() ) {
					if ( cmd.command() == (*it)->myCommand() ) {
						(*it)->onProtocol(cmd);
						break;
					}
					it++;
				}
			} else if ( DDENUM_COMMAND_APP == cmd.type() ) {
				std::list<ddSrvManager::notifier *>::const_iterator it = m_listNotifier.begin();
				while ( it != m_listNotifier.end() ) {
					if ( (*it)->isMyCommand(cmd.command()) ) {
						(*it)->onProtocol(cmd);
						break;
					}
					it++;
				}
			}
		}
	}

private:
	std::list<ddSrvManager::listener *> m_listListener;
	std::list<ddSrvManager::notifier *> m_listNotifier;
};

///////////////////////////////////////////////////////////////////////////////
ddSrvManager::listener::listener()
{
	DD_GLOBAL_INSTANCE_DO(ddSrvManager, add(this));
}


ddSrvManager::notifier::notifier()
{
	DD_GLOBAL_INSTANCE_DO(ddSrvManager, add(this));
}

ddSrvManager::ddSrvManager()
{
	DD_D_NEW(ddSrvManagerPrivate);
}

ddSrvManager::~ddSrvManager()
{
	DD_D_DELETE();
}

ddVoid ddSrvManager::add(listener* pListener)
{
	dPtr()->add(pListener);
}

ddVoid ddSrvManager::remove(listener *pListener)
{
	dPtr()->remove(pListener);
}

ddVoid ddSrvManager::add(notifier* pNotifier)
{
	dPtr()->add(pNotifier);
}

ddVoid ddSrvManager::remove(notifier *pNotifier)
{
	dPtr()->remove(pNotifier);
}

ddVoid ddSrvManager::notify(ddpCByte data, ddUInt16 len)
{
	dPtr()->notify(data, len);
}

