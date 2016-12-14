/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_thread.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-13 14:30      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_thread_h
#define dd_thread_h

class ddThreadPrivate;
class ddThread {
	DD_PRIVATE_DECLARE(ddThread)
public:
	class interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onThread(ddThread* pThread) = 0;
	};
	ddThread(interface *pOwner);
	~ddThread();
	ddVoid setOwner(interface *pOwner);
	ddBool create();
	ddVoid exit();
	static ddVoid sleep(ddUInt32 s);
	static ddVoid msleep(ddUInt32 ms);
	static ddVoid usleep(ddUInt64 us);
};

///////////////////////////////////////////////////////////////////////////////
class ddMutexPrivate;
class ddMutex {
	DD_PRIVATE_DECLARE(ddMutex)
public:
	ddMutex();
	~ddMutex();
	ddVoid lock();
	ddVoid tryLock();
	ddVoid unlock();
};

///////////////////////////////////////////////////////////////////////////////
class ddCondPrivate;
class ddCond {
	DD_PRIVATE_DECLARE(ddCond)
public:
	ddCond();
	~ddCond();
	ddVoid signal();
	ddVoid broadcast();
	ddVoid wait(ddMutex* pMutex);
	ddBool waitUntil(ddMutex *pMutex, ddUInt32 ms);
};
#endif // dd_thread_h

