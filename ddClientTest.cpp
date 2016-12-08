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

class ddClientTest: public ddApp, public ddTimer::interface {
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
			ddUInt8 data = 88;
//			ddService::ioctl(1, &data, sizeof(ddUInt8));
			ddService::ioctl(1, &data, sizeof(ddUInt8), &data, sizeof(ddUInt8));
		}
		return 0;
	}

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

