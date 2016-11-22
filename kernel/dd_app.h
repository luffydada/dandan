/***************************************************************************
*    File name:        dd_app.h
*    Description:
*    Create date:      2016-11-22 21:47
*    Modified date:    2016-11-22 21:47
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
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
