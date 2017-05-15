/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_mediadevice.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-14 13:51      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <sys/socket.h>
#include <linux/netlink.h>

#include "../dandan.h"

ddpCChar PATTERN = "block/sd[a-z]/sd[a-z][1-9]?$";
class ddMediaDevicePrivate : public ddPrivateBase, public ddThread::interface {
	DD_PUBLIC_DECLARE(ddMediaDevice)
public:
	ddMediaDevicePrivate() : m_pOwner(nil), m_socketFd(-1), m_thread(this), m_isExit(no), m_reg(PATTERN) {
		FD_ZERO(&m_fds);
		m_thread.create();
	}

	~ddMediaDevicePrivate() {
		m_isExit = yes;
		m_mutex.lock();
		m_cond.signal();
		m_mutex.unlock();
		if ( -1 != m_socketFd ) {
			close(m_socketFd);
			m_socketFd = -1;
		}
	}
	
	virtual ddVoid onThread(ddThread* pThread) {
		if ( pThread == &m_thread ) {
			m_reg.compile();
			ddChar data[1024] = {0};
			m_mutex.lock();
			m_cond.wait(&m_mutex);
			while ( !m_isExit ) {
				select(m_socketFd + 1, &m_fds, nil, nil, nil);  
				/* receive data */  
				memset(data, 0, sizeof(data));
				if ( 0 < recv(m_socketFd, &data, sizeof(data), 0) ) {  
					parseData(data);
				}  
			}
			m_mutex.unlock();
		}
	}

	ddVoid setOwner(ddMediaDevice::interface *pOwner) {
		m_pOwner = pOwner;
	}

	ddBool start() {
		if ( -1 == m_socketFd ) {
			m_socketFd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT); 
			if ( -1 ==  m_socketFd ) {
				dd_log_w("socket,failed,error:%s\n", strerror(errno));
				return no;
			}
			ddUInt16 bufferSize = 1024;
			setsockopt(m_socketFd, SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
			struct sockaddr_nl addr;
			memset(&addr, 0, sizeof(addr));
		    addr.nl_family = AF_NETLINK;
		    addr.nl_pid = getpid();
			addr.nl_groups = 1; /* receive broadcast message*/ 
			if ( -1 == bind(m_socketFd, (struct sockaddr*)&addr, sizeof(addr)) ) {
				dd_log_w("bind,failed,error:%s\n", strerror(errno));
				close(m_socketFd);
				m_socketFd = -1;
				return no;
			}
			FD_SET(m_socketFd, &m_fds);
			m_mutex.lock();
			m_cond.signal();
			m_mutex.unlock();
		}
		return no;
	}

	ddVoid parseData(ddpCChar data) {
		if ( data && strlen(data) ) {
			if ( m_reg.exec(data) ) {
				if ( !strncmp(data, "add@", 4) ) {
					if ( m_pOwner ) {
						m_pOwner->onMediadevice_attached(DDENUM_MEDIADEVICE_USB);
					}
				} else if ( !strncmp(data, "change@", 7) ) {
				} else if ( !strncmp(data, "remove@", 7) ) {
					if ( m_pOwner ) {
						m_pOwner->onMediadevice_detached(DDENUM_MEDIADEVICE_USB);
					}
				}
			}
		}
	}

private:
	ddMediaDevice::interface *m_pOwner;
	int m_socketFd;
	fd_set m_fds; 
	ddMutex m_mutex;
	ddCond m_cond;
	ddThread m_thread;
	ddBool m_isExit;
	ddRegular m_reg;
};

///////////////////////////////////////////////////////////////////////////////
ddMediaDevice::ddMediaDevice(interface *pOwner)
{
	DD_D_NEW(ddMediaDevicePrivate);
	setOwner(pOwner);
}

ddMediaDevice::~ddMediaDevice()
{
	DD_D_DELETE();
}

ddVoid ddMediaDevice::setOwner(interface *pOwner)
{
	dPtr()->setOwner(pOwner);
}

ddBool ddMediaDevice::start()
{
	return dPtr()->start();
}

