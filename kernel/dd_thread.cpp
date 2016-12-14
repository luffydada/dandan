/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_thread.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-13 14:30      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <glib-2.0/glib.h>

#include "../dandan.h"
class ddThreadPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddThread)
public:
	ddThreadPrivate(): m_pOwner(nil), m_pThread(nil) {
	}

	~ddThreadPrivate() {
		if ( m_pThread ) {
			g_thread_unref(m_pThread);
			m_pThread = nil;
		}
	}

	static gpointer onGThreadFunc(gpointer data) {
		ddThreadPrivate *pthis = (ddThreadPrivate *)data;
		if ( pthis->m_pOwner ) {
			pthis->m_pOwner->onThread(pthis->bPtr());
		}
		return gpointer(0);
	}

	ddVoid setOwner(ddThread::interface *pOwner) {
		m_pOwner = pOwner;
	}

	ddBool create() {
		if ( m_pThread ) {
			g_thread_join(m_pThread);
		}
		GError *error = nil;
		m_pThread == g_thread_try_new("__dd_thread__", onGThreadFunc, this, &error);
		if ( error ) {
			dd_log_w("createThread failed:%s", error->message);
			g_error_free(error);
			error = nil;
		}
		return (m_pThread ? yes : no);
	}

	ddVoid exit() {
		g_thread_exit(gpointer(0));
	}

private:
	ddThread::interface *m_pOwner;
	GThread *m_pThread;
};

ddThread::ddThread(interface *pOwner)
{
	DD_D_NEW(ddThreadPrivate);
	setOwner(pOwner);
}

ddThread::~ddThread()
{
	DD_D_DELETE();
}

ddVoid ddThread::setOwner(interface *pOwner)
{
	dPtr()->setOwner(pOwner);
}

ddBool ddThread::create()
{
	return dPtr()->create();
}

ddVoid ddThread::exit()
{
	return dPtr()->exit();
}

ddVoid ddThread::sleep(ddUInt32 s)
{
	::sleep(s);
}

ddVoid ddThread::msleep(ddUInt32 ms)
{
	::usleep(ms * 1000);
}

ddVoid ddThread::usleep(ddUInt64 us)
{
	::usleep(us);
}

///////////////////////////////////////////////////////////////////////////////
class ddMutexPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddMutex)
public:
	ddMutexPrivate() { g_mutex_init(&m_mutex); }
	~ddMutexPrivate() { g_mutex_clear(&m_mutex); }
	ddVoid lock() { g_mutex_lock(&m_mutex); }
	ddVoid tryLock() { g_mutex_trylock(&m_mutex); }
	ddVoid unlock() { g_mutex_unlock(&m_mutex); }
	GMutex* mutex() { return &m_mutex; }
private: GMutex m_mutex;
};

ddMutex::ddMutex()
{
	DD_D_NEW(ddMutexPrivate);
}

ddMutex::~ddMutex()
{
	DD_D_DELETE();
}

ddVoid ddMutex::lock()
{
	dPtr()->lock();
}

ddVoid ddMutex::tryLock()
{
	dPtr()->tryLock();
}

ddVoid ddMutex::unlock()
{
	dPtr()->unlock();
}

///////////////////////////////////////////////////////////////////////////////
class ddCondPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddCond)
public:
	friend ddMutexPrivate;
	ddCondPrivate() { g_cond_init(&m_cond); }
	~ddCondPrivate() { g_cond_clear(&m_cond); }
	ddVoid signal() { g_cond_signal(&m_cond); }
	ddVoid broadcast() { g_cond_broadcast(&m_cond); }
	ddVoid wait(ddMutex* pMutex) { 
		if ( pMutex ) {
			g_cond_wait(&m_cond, pMutex->dPtr()->mutex());
		}
	}

	ddBool waitUntil(ddMutex *pMutex, ddUInt32 ms) {
		ddBool ret = no;
		if ( pMutex ) {
			ret = g_cond_wait_until(&m_cond, pMutex->dPtr()->mutex(), g_get_monotonic_time() + 1000 * ms);
		}
		return ret;
	}
private:
	GCond m_cond;
};

ddCond::ddCond()
{
	DD_D_NEW(ddCondPrivate);
}

ddCond::~ddCond()
{
	DD_D_DELETE();
}

ddVoid ddCond::signal()
{
	dPtr()->signal();
}

ddVoid ddCond::broadcast()
{
	dPtr()->broadcast();
}

ddVoid ddCond::wait(ddMutex* pMutex)
{
	dPtr()->wait(pMutex);
}

ddBool ddCond::waitUntil(ddMutex *pMutex, ddUInt32 ms)
{
	return dPtr()->waitUntil(pMutex, ms);
}

