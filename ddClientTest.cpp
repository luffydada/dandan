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
class ddClientTest: public ddApp, public ddTimer::interface, public ddSrvManager::/*listener*/notifier, ddThread::interface {
public:
	ddClientTest() : m_testTimer(this), m_testThread1(this), m_testThread2(this) {
		m_testTimer.setTimer(3000);
	}

	~ddClientTest() {
	}

	virtual ddVoid onInitApp() {
		dd_log_d("ddClientTest,onInitApp\n");
		m_testThread1.start();
	}

	virtual ddVoid onTimer(ddTimer* pTimer) {
		if ( pTimer == &m_testTimer ) {
/*			ddUInt8 data = 88;
			ddService::ioctl(DDDEF_IOCOMMAND_RADIO + 1, &data, sizeof(ddUInt8), &data, sizeof(ddUInt8));
			dd_log_d("ddClientTest,ioctl,pout:%d\n", data);
*/
					ddUInt8 ok = 33;
					ddCommand cmd(this, DDDEF_IOCOMMAND_RADIO, DDENUM_COMMAND_SERVICE, &ok, 1);
					cmd.download();
			m_testThread1.start();
			m_testThread2.start();
		}
	}
		
	virtual ddBool isMyCommand(ddUInt16 command) {
		return command > DDDEF_IOCOMMAND_RADIO && command < DDDEF_IOCOMMAND_BLUETOOTH;
	}

	virtual ddVoid onProtocol(ddCommand& cmd) {
		dd_log_d("ddClientTest,onProtocol,cmd:%d\n", cmd.command());
	}
/*
	virtual ddUInt16 myCommand() {
		return DDDEF_IOCOMMAND_RADIO;
	}

	virtual ddVoid onProtocol(ddCommand& cmd) {
		dd_log_d("ddClientTest,onProtocol,cmd:%d\n", cmd.command());
		if ( DDDEF_IOCOMMAND_RADIO == cmd.command() ) {
			dd_log_d("ddClientTest,onProtocol,data:%d\n", *cmd.data());
		}
	}
*/
	virtual ddVoid onThread(ddThread* pThread) {
		if ( &m_testThread1 == pThread ) {
			dd_log_d("test thread1 run\n");
		} else if ( &m_testThread2 == pThread ) {
			dd_log_d("test thread2 run\n");
		}
	}

private:
	ddTimer m_testTimer;
	ddThread m_testThread1;
	ddThread m_testThread2;
};

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	ddClientTest client;
	ddClientTest::startup("client");
	return client.run();
}

