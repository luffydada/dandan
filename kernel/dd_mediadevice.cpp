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
//#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#include "../dandan.h"

class ddMediaDevicePrivate : public ddPrivateBase, public ddThread::interface {
	DD_PUBLIC_DECLARE(ddMediaDevice)
public:
	ddMediaDevicePrivate() : m_socketFd(-1), m_thread(this), m_isExit(no) {
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
			ddChar data[1024] = {0};
			m_mutex.lock();
			m_cond.wait(&m_mutex);
			while ( !m_isExit ) {
				select(m_socketFd + 1, &m_fds, nil, nil, nil);  
				/* receive data */  
				memset(data, 0, sizeof(data));
				ssize_t rcvlen = recv(m_socketFd, &data, sizeof(data), 0);  
				if (rcvlen > 0) {  
					dd_log_i("netlink recv:%s\n", data);
				}  
			}
			m_mutex.unlock();
		}
	}

	ddBool start() {
		if ( -1 == m_socketFd ) {
			m_socketFd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT); 
			if ( -1 ==  m_socketFd ) {
				dd_log_w("socket,failed,error:%s\n", strerror(errno));
				return no;
			}
			ddUInt16 bufferSize = 512;
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

private:
	int m_socketFd;
	fd_set m_fds; 
	ddMutex m_mutex;
	ddCond m_cond;
	ddThread m_thread;
	ddBool m_isExit;
};

///////////////////////////////////////////////////////////////////////////////
ddMediaDevice::ddMediaDevice()
{
	DD_D_NEW(ddMediaDevicePrivate);
}

ddMediaDevice::~ddMediaDevice()
{
	DD_D_DELETE();
}

ddBool ddMediaDevice::start()
{
	return dPtr()->start();
}

