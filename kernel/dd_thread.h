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
	ddBool start();
	ddVoid exit();
};

#endif // dd_thread_h

