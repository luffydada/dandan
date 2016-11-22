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

class ddClientTest: public ddApp {
public:
	ddClientTest()
	{
	}

	~ddClientTest()
	{
	}

	virtual ddVoid onInitApp()
	{
		printf("dddClientTest,onInitApp\n");
	}
};

int main(int argc, char *argv[])
{
	ddClientTest client;
	client.run();
	return 0;
}

