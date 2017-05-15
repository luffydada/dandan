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
class ddApp : public ddCommand::interface {
	DD_PRIVATE_DECLARE(ddApp)
public:
	class interface : public ddCommand::interface {
	public:
		virtual ddVoid onDownload(ddpCByte data, ddUInt16 len);
	};
	ddApp();
	virtual ~ddApp();

	/* 实现ddCommand::interface的接口 */
	ddVoid onDownload(ddpCByte data, ddUInt16 len);

	/* 自定义函数 */
	virtual ddVoid onInitApp();
	static ddVoid startup(ddpCChar pName, ddBool isServer = no);
	static ddApp* sharedApp();
	ddInt run();
	ddVoid quit();
};

#endif // dd_app_h
