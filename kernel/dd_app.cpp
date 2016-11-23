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
		m_timer.setTimer(5);
	}

	~ddAppPrivate() {
		if ( m_pMainLoop ) {
			g_main_loop_unref(m_pMainLoop);
		}
	}

	ddVoid run() {
		g_main_loop_run(m_pMainLoop);
	}

	ddVoid quit() {
		g_main_loop_quit(m_pMainLoop);
	}

	virtual ddUInt onTimer(ddUInt uTimerId) {
		if ( m_timer.isMe(uTimerId) ) {
			bPtr()->onInitApp();
		}
		return 0;
	}

private:
	GMainLoop *m_pMainLoop;
	ddTimer m_timer;
};

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

ddVoid ddApp::run()
{
	dPtr()->run();
}

ddVoid ddApp::quit()
{
	dPtr()->quit();
}
