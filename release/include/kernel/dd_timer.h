/***************************************************************************
*    File name:        dd_timer.h
*    Description:
*    Create date:      2016-11-23 21:38
*    Modified date:    2016-11-23 21:38
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#ifndef dd_timer_h
#define dd_timer_h

class ddTimerPrivate;
class ddTimer {
	DD_PRIVATE_DECLARE(ddTimer)
public:
	class interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onTimer(ddTimer* pTimer) = 0;
	};

	ddTimer(interface *pOwner = nil);
	virtual ~ddTimer();
	ddVoid setOwner(interface *pOwner);
	ddVoid setTimer(ddUInt uTimeout, ddBool isLoop = no);
	ddVoid killTimer();
};

#endif // dd_timer_h

