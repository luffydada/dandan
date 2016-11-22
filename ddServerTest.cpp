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

class ddServerTest : public ddApp {
public:
	ddServerTest()
	{
	}

	~ddServerTest()
	{
	}

	virtual ddVoid onInitApp()
	{
		printf("ddServerTest,onInitApp\n");
	}
};

int main(int argc, char *argv[])
{
	ddServerTest server;
	server.run();
	return 0;
}

