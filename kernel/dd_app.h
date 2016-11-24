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

class ddAppPrivate;
class ddApp {
	DD_PRIVATE_DECLARE(ddApp)
public:
	ddApp();
	~ddApp();
	virtual ddVoid onInitApp();
	static ddVoid startup(ddpCChar pName, ddBool isServer = no);
	ddInt run();
	ddVoid quit();
};

#endif // dd_app_h
