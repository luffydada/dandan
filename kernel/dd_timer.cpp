/***************************************************************************
*    File name:        dd_timer.cpp
*    Description:
*    Create date:      2016-11-23 21:38
*    Modified date:    2016-11-23 21:38
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#include <glib-2.0/glib.h>
#include "../dandan.h"

class ddTimerPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddTimer)
public:
	static gboolean onTimeoutCallback(gpointer user_data) {
		ddTimerPrivate *pthis = (ddTimerPrivate *)user_data;
		if ( !pthis->m_isLoop ) {
			pthis->m_id = 0;
		}
		pthis->onTimer();
		return pthis->m_isLoop;
	}

	ddTimerPrivate() :m_pOwner(nil), m_timerId(0), m_isLoop(no) {
		m_timerId = ddTimer::realTimerId();
	}

	~ddTimerPrivate() {
		killTimer();
	}

	ddVoid setOwner(ddTimer::interface *pOwner) {
		m_pOwner = pOwner;
	}

	ddVoid setTimer(ddUInt uTimeout, ddBool isLoop) {
		killTimer();
		m_id = g_timeout_add(uTimeout, onTimeoutCallback, this);
		m_isLoop = isLoop;
	}

	ddVoid killTimer() {
		if ( m_id ) {
			g_source_remove(m_id);
			m_id = 0;
		}
	}

	ddVoid onTimer() {
		if ( m_pOwner ) {
			m_pOwner->onTimer(m_timerId);
		}
	}

private:
	ddTimer::interface *m_pOwner;
	guint m_id;
	ddUInt m_timerId;
	ddBool m_isLoop;
};

///////////////////////////////////////////////////////////////////////////////
ddTimer::ddTimer(interface *pOwner/* = nil*/)
{
	DD_D_NEW(ddTimerPrivate);
	setOwner(pOwner);
}

ddTimer::~ddTimer()
{
	DD_D_DELETE();
}

ddUInt ddTimer::realTimerId()
{
	static ddUInt id = 0;
	return ++id;
}

ddVoid ddTimer::setOwner(interface *pOwner)
{
	dPtr()->setOwner(pOwner);
}

ddBool ddTimer::isMe(ddUInt uTimerId)
{
	return (uTimerId == dPtr()->m_timerId);
}

ddVoid ddTimer::setTimer(ddUInt uTimeout, ddBool isLoop/* = no*/)
{
	dPtr()->setTimer(uTimeout, isLoop);
}

ddVoid ddTimer::killTimer()
{
	dPtr()->killTimer();
}

