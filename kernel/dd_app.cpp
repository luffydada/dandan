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

class ddAppPrivate {
private:
	static ddAppPrivate *s_pThis;
	static ddApp *s_pOwn;
public:
	static ddAppPrivate *getInstance()
	{
		if ( !s_pThis ) {
			s_pThis = new ddAppPrivate();
		}
		return s_pThis;
	}
	void release()
	{
		if ( s_pThis ) {
			delete s_pThis;
			s_pThis = nil;
		}
	}
	static void setOwn(ddApp* pOwn)
   	{
		s_pOwn = pOwn;
	}

public:
	ddAppPrivate()
   	{
		m_pMainLoop = g_main_loop_new(nil, no);
	}

	~ddAppPrivate()
   	{
		if ( m_pMainLoop ) {
			g_main_loop_unref(m_pMainLoop);
		}
		release();
	}

	ddVoid run()
	{
		if ( s_pOwn ) {
			s_pOwn->onInitApp();
		}
		g_main_loop_run(m_pMainLoop);
	}

	ddVoid quit()
	{
		g_main_loop_quit(m_pMainLoop);
	}

private:
	GMainLoop *m_pMainLoop;
};

ddAppPrivate *ddAppPrivate::s_pThis = nil;
ddApp *ddAppPrivate::s_pOwn = nil;

ddApp::ddApp()
{
	ddAppPrivate::getInstance();
}

ddApp::~ddApp()
{
}

ddVoid ddApp::onInitApp()
{
}

ddVoid ddApp::run()
{
	ddAppPrivate::getInstance()->run();
}

ddVoid ddApp::quit()
{
	ddAppPrivate::getInstance()->quit();
}
