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
		virtual ddUInt onTimer(ddUInt uTimerId) { return uTimerId; }
	};

	ddTimer(interface *pOwner = nil);
	virtual ~ddTimer();
	static ddUInt realTimerId();
	ddVoid setOwner(interface *pOwner);
	ddBool isMe(ddUInt uTimerId);
	ddVoid setTimer(ddUInt uTimeout, ddBool isLoop = no);
	ddVoid killTimer();
};

#endif // dd_timer_h

