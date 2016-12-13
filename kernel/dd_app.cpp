/***************************************************************************
*    File name:        dd_app.cpp
*    Description:
*    Create date:      2016-11-22 21:47
*    Modified date:    2016-11-22 21:47
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#include <glib-2.0/glib.h>

#include "../dandan.h"

class ddAppPrivate : public ddPrivateBase, public ddTimer::interface {
	DD_PUBLIC_DECLARE(ddApp)
public:
	ddAppPrivate() :m_timer(this) {
		m_pMainLoop = g_main_loop_new(nil, no);
		m_pMainContext = g_main_loop_get_context(m_pMainLoop);
		m_timer.setTimer(5);
	}

	~ddAppPrivate() {
		if ( m_pMainContext ) {
			g_main_context_unref(m_pMainContext);
		}
		if ( m_pMainLoop ) {
			g_main_loop_unref(m_pMainLoop);
		}
	}

	ddInt run() {
		g_main_loop_run(m_pMainLoop);
		return 0;
	}

	ddVoid quit() {
		g_main_loop_quit(m_pMainLoop);
	}

	virtual ddVoid onTimer(ddTimer* pTimer) {
		if ( pTimer == &m_timer ) {
			bPtr()->onInitApp();
		}
	}

private:
	GMainLoop *m_pMainLoop;
	GMainContext* m_pMainContext;
	ddTimer m_timer;
};

///////////////////////////////////////////////////////////////////////////////
ddApp::ddApp()
{
	DD_D_NEW(ddAppPrivate);
}

ddApp::~ddApp()
{
	DD_D_DELETE();
}

ddVoid ddApp::onInitApp()
{
}

ddVoid ddApp::startup(ddpCChar pName, ddBool isServer/* = no*/)
{
	ddService::startup(pName, isServer);
}

ddInt ddApp::run()
{
	return dPtr()->run();
}

ddVoid ddApp::quit()
{
	dPtr()->quit();
}

ddVoid ddApp::onDownload(ddpCByte data, ddUInt16 len)
{
	ddService::download(data, len);
}

