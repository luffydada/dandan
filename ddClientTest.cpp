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
class ddClientTest: public ddApp, public ddTimer::interface, public ddSrvManager::/*listener*/notifier
	, public ddThread::interface
	, public ddMediaDevice::interface
	, public ddMediaPlayer::interface {
public:
	ddClientTest() : m_testTimer(this), m_testThread1(this), m_testThread2(this), m_mediaDevice(this), m_mediaPlayer(this){
		m_testTimer.setTimer(3000);
	}

	~ddClientTest() {
	}

	virtual ddVoid onInitApp() {
		dd_log_d("ddClientTest,onInitApp\n");
		m_testThread1.create();
		m_mediaDevice.start();
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
			m_testThread1.create();
			m_testThread2.create();
//			m_mediaPlayer.playFile("/media/eddy/WANGYING/video/MKV/400x240.mkv");
//			m_mediaPlayer.playFile("/media/eddy/WANGYING/music/AAC/陈小春-没那种命.aac");
			m_mediaPlayer.playFile("/media/eddy/WANGYING/music/带封面的歌曲/北京欢迎你-群星.mp3");
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

	virtual ddVoid onMediadevice_attached(emMediaDeviceType device) {
		dd_log_d("onMediadevice_attached,device:%d\n", device);
	}

	virtual ddVoid onMediadevice_detached(emMediaDeviceType device) {
		dd_log_d("onMediadevice_detached,device:%d\n", device);
	}

	virtual ddVoid onMediaPlayer_process(ddUInt32 current, ddUInt32 duration) {
		dd_log_d("onMediaPlayer_process,current:%d,duration:%d\n", current, duration);
	}

	virtual ddVoid onMediaPlayer_playState(emMediaPlayState state) {
		dd_log_d("onMediaPlayer_playState,state:%d\n", state);
	}

	virtual ddVoid onMediaPlayer_id3Info(ddpCChar pTitle, ddpCChar pArtist, ddpCChar pAlbum) {
		dd_log_d("onMediaPlayer_id3Info,pTitle:%s,pArtist:%s,pAlbum:%s\n", pTitle, pArtist, pAlbum);
	}

	virtual ddVoid onMediaPlayer_playEnd() {
		dd_log_d("onMediaPlayer_playEnd\n");
	}

	virtual ddVoid onMediaPlayer_playError(ddpCChar pError) {
		dd_log_d("onMediaPlayer_playError,error:%s\n", pError);
	}

	virtual ddVoid onMediaPlayer_playWarning(ddpCChar pWarning) {
		dd_log_d("onMediaPlayer_playError,warning:%s\n", pWarning);
	}

private:
	ddTimer m_testTimer;
	ddThread m_testThread1;
	ddThread m_testThread2;
	ddMediaDevice m_mediaDevice;
	ddMediaPlayer m_mediaPlayer;
};

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	ddClientTest client;
	ddClientTest::startup("client");
	return client.run();
}

