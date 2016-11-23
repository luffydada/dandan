/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_timer.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-11-23 14:47      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_timer_h
#define dd_timer_h

class ddTimerPrivate;
class ddTimer {
	DD_PRIVATE_DECLARE(ddTimer)
public:
	class DANDAN_EXPORT interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onTimer(ddUInt uTimerId) {}
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

