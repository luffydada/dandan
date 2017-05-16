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
#include "../dandan.h"

class ddRadioDevice : public ddDevice {
public:
	enum {
		DDENUM_IOCOMMAND_RADIO_START = DDDEF_IOCOMMAND_RADIO,
		DDENUM_IOCOMMAND_RADIO_TEST,
		DDENUM_IOCOMMAND_RADIO_END
	};
	ddRadioDevice() {
	}

	~ddRadioDevice() {
	}

	virtual emDeviceId deviceId() {
		return DDENUM_DEVICEID_RADIO;
	}

	virtual ddBool isMyCommand(ddUInt16 iocmd) {
		return iocmd > DDENUM_IOCOMMAND_RADIO_START && iocmd < DDENUM_IOCOMMAND_RADIO_END;
	}

	virtual ddBool isMyProtocol(ddUInt16 iocmd) {
		return iocmd == DDDEF_IOCOMMAND_RADIO;
	}

	virtual ddVoid onIoctl(ddUInt16 iocmd, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout) {
		dd_log_d("radio device,onIoctl,iocmd:%d\n", iocmd);
		switch ( iocmd ) {
			case DDENUM_IOCOMMAND_RADIO_TEST:
				if ( pin && uin && pout && uout ) {
					dd_log_d("radio device,onIoctl,test,pin:%d\n", *(ddUInt8 *)pin);
					*(ddUInt8 *)pout = *(ddUInt8 *)pin + 1;
					ddUInt8 ok = 11;
					ddCommand cmd(this, DDDEF_IOCOMMAND_RADIO + 1, DDENUM_COMMAND_APP, 1, &ok);
					cmd.commit();
				}
				break;
			default: break;
		}
	}

	virtual ddVoid onMessage(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam) {
	}

	virtual ddVoid onProtocol(ddCommand& cmd) {
		if ( DDDEF_IOCOMMAND_RADIO == cmd.command() ) {
			dd_log_d("ddRadioDevice,onProtocol,data:%d\n", *cmd.data());
			ddUInt8 ok = 22;
			ddCommand cmd2(this, DDDEF_IOCOMMAND_RADIO, DDENUM_COMMAND_SERVICE, 1, &ok);
			cmd2.download();
		}
	}
};

class ddBluetoothDevice : public ddDevice {
public:
	enum {
		DDENUM_IOCOMMAND_BLUETOOTH_START = DDDEF_IOCOMMAND_BLUETOOTH,
		DDENUM_IOCOMMAND_BLUETOOTH_TEST,
		DDENUM_IOCOMMAND_BLUETOOTH_END
	};
	ddBluetoothDevice() {
	}

	~ddBluetoothDevice() {
	}

	virtual emDeviceId deviceId() {
		return DDENUM_DEVICEID_BLUETOOTH;
	}

	virtual ddBool isMyProtocol(ddUInt16 iocmd) {
		return iocmd == DDDEF_IOCOMMAND_BLUETOOTH;
	}

	virtual ddBool isMyCommand(ddUInt16 iocmd) {
		return iocmd > DDENUM_IOCOMMAND_BLUETOOTH_START && iocmd < DDENUM_IOCOMMAND_BLUETOOTH_END;
	}

	virtual ddVoid onIoctl(ddUInt16 iocmd, ddCPointer pin, ddUInt16 uin, ddPointer pout, ddUInt16 uout) {
		dd_log_d("bluetooth device,onIoctl\n");
		switch ( iocmd ) {
			case DDENUM_IOCOMMAND_BLUETOOTH_TEST:
				break;
			default: break;
		}
	}

	virtual ddVoid onMessage(ddUInt32 msg, ddUInt32& wParam, ddUInt32& lParam) {
	}

	virtual ddVoid onProtocol(ddCommand& cmd) {
		if ( DDDEF_IOCOMMAND_BLUETOOTH == cmd.command() ) {
			dd_log_d("ddBluetoothDevice,onProtocol,data:%d\n", *cmd.data());
		}
	}
};

class ddServerTest : public ddApp, public ddTimer::interface {
public:
	ddServerTest() : timer1(this), timer2(this) {
		timer1.setTimer(1000);
	}

	~ddServerTest() {
	}

	virtual ddVoid onInitApp() {
		dd_log_d("ddServerTest,onInitApp\n");
		DD_GLOBAL_INSTANCE_DO(ddDevManager, add(ddGlobalInstance<ddRadioDevice>::instance()));
		DD_GLOBAL_INSTANCE_DO(ddDevManager, add(ddGlobalInstance<ddBluetoothDevice>::instance()));
		timer2.setTimer(1000);
	}
		
	virtual ddVoid onTimer(ddTimer* pTimer) {
		if ( pTimer == &timer1 ) {
			dd_log_d("ddServerTest,onTimer,time1\n");
		} else if ( pTimer == &timer2 ) {
			dd_log_d("ddServerTest,onTimer,time2\n");
		}
	}

private:
	ddTimer timer1;
	ddTimer timer2;
};

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	ddServerTest::startup("client", yes);
	ddServerTest server;
	return server.run();
}

