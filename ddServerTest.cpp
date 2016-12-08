/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    ddServiceTest.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-11-22 15:17      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include "dandan.h"

class ddServerTest : public ddApp, public ddTimer::interface {
public:
	ddServerTest() : timer1(this) {
		timer1.setTimer(1000);
	}

	~ddServerTest() {
	}

	virtual ddVoid onInitApp() {
		printf("ddServerTest,onInitApp\n");
	}
		
	virtual ddUInt onTimer(ddUInt uTimerId) {
		if ( timer1.isMe(uTimerId) ) {
			printf("ddServerTest,onTimer,time1\n");
		}
		return 0;
	}

private:
	ddTimer timer1;
};

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	ddServerTest::startup("client", yes);
	ddServerTest server;
	return server.run();
}

