/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_command.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-08 17:40      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_command_h
#define dd_command_h

typedef enum {
	DDENUM_COMMAND_SERVICE,
	DDENUM_COMMAND_APP,
}emCommandType;

class ddCommand {
public:
	class interface {
	public:
		virtual ~interface();
		virtual ddVoid onCommit(ddpCByte data, ddUInt16 len);
		virtual ddVoid onDownload(ddpCByte data, ddUInt16 len);
	};
	ddCommand(interface *pOwner, ddUInt16 cmd = 0, emCommandType type = DDENUM_COMMAND_SERVICE, ddpCByte data = nil, ddUInt16 len = 0);
	ddCommand(ddpCByte data, ddUInt16 len);
	~ddCommand();
	ddVoid setCommand(ddUInt16 cmd);
	ddUInt16 command();
	ddVoid setType(emCommandType type);
	emCommandType type();
	ddVoid setData(ddpCByte data, ddUInt16 len, ddUInt16 pos = 0);
	ddpCByte data();
	ddUInt16 length();
	ddUInt16 maxLength();
	ddVoid download();
	ddVoid commit();

private:
	ddCommand::interface *m_pOwner;
	ddByte m_data[DD_MAXDATA];
	ddUInt16 m_len;
};

#endif // dd_command_h

