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

class ddServerTest : public ddApp , public ddTimer::interface {
public:
	ddServerTest() : timer1(this), timer2(this) {
		timer1.setTimer(1000);
		timer2.setTimer(1000, yes);
	}

	~ddServerTest() {
	}

	virtual ddVoid onInitApp() {
		printf("ddServerTest,onInitApp\n");
	}
		
	virtual ddVoid onTimer(ddUInt uTimerId) {
		if ( timer1.isMe(uTimerId) ) {
			printf("ddServerTest,onTimer,time1\n");
		} else if ( timer2.isMe(uTimerId) ) {
			printf("ddServerTest,onTimer,time2\n");
		}
	}

private:
	ddTimer timer1;
	ddTimer timer2;
};

int main(int argc, char *argv[])
{
	ddServerTest server;
	server.run();
	return 0;
}

