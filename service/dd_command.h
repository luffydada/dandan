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

class ddCommandPrivate;
class ddCommand {
	DD_PRIVATE_DECLARE(ddCommand)
public:
	class interface {
	public:
		interface();
		virtual ~interface();
		virtual ddVoid onCommit(ddpCByte data, ddUInt16 len);
		virtual ddVoid onDownload(ddpCByte data, ddUInt16 len);
	};
	ddCommand(interface *pOwner);
	~ddCommand();
	ddVoid setData(ddpCByte data, ddUInt16 len, ddUInt16 pos = 0);
	ddpCByte data();
	ddUInt16 length();
	ddUInt16 maxLength();
	ddVoid download();
	ddVoid commit();
};
#endif // dd_command_h

