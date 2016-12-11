/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    ddClientTest.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-11-22 15:17      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include "dandan.h"
class ddClientTest: public ddApp, public ddTimer::interface, public ddSrvManager::/*listener*/notifier {
public:
	ddClientTest() : m_testTimer(this) {
		m_testTimer.setTimer(3000);
	}

	~ddClientTest() {
	}

	virtual ddVoid onInitApp() {
		printf("ddClientTest,onInitApp\n");
	}
	virtual ddUInt onTimer(ddUInt uTimerId) {
		if ( m_testTimer.isMe(uTimerId) ) {
/*			ddUInt8 data = 88;
			ddService::ioctl(DDDEF_IOCOMMAND_RADIO + 1, &data, sizeof(ddUInt8), &data, sizeof(ddUInt8));
			printf("ddClientTest,ioctl,pout:%d\n", data);
*/
					ddUInt8 ok = 33;
					ddCommand cmd(this, DDDEF_IOCOMMAND_RADIO, DDENUM_COMMAND_SERVICE, &ok, 1);
					cmd.download();
		}
		return 0;
	}
		
	virtual ddBool isMyCommand(ddUInt16 command) {
		return command > DDDEF_IOCOMMAND_RADIO && command < DDDEF_IOCOMMAND_BLUETOOTH;
	}

	virtual ddVoid onProtocol(ddCommand& cmd) {
		printf("ddClientTest,onProtocol,cmd:%d\n", cmd.command());
	}
/*
	virtual ddUInt16 myCommand() {
		return DDDEF_IOCOMMAND_RADIO;
	}

	virtual ddVoid onProtocol(ddCommand& cmd) {
		printf("ddClientTest,onProtocol,cmd:%d\n", cmd.command());
		if ( DDDEF_IOCOMMAND_RADIO == cmd.command() ) {
			printf("ddClientTest,onProtocol,data:%d\n", *cmd.data());
		}
	}
*/
private:
	ddTimer m_testTimer;
};

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	ddClientTest client;
	ddClientTest::startup("client");
	return client.run();
}

