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

	ddBool start() {
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
///////////////////////////////////////////////////////////////////////////////

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

ddBool ddThread::start()
{
	return dPtr()->start();
}

ddVoid  ddThread::exit()
{
	return dPtr()->exit();
}

