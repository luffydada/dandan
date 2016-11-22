/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_app.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-11-22 14:13      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_app_h
#define dd_app_h

class ddApp {
public:
	ddApp();
	~ddApp();
	virtual ddVoid onInitApp();
	ddVoid run();
	ddVoid quit();
};
#endif // dd_app_h
