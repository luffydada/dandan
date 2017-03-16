/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_mediaplayer.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-22 15:54      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_mediaplayer_h
#define dd_mediaplayer_h

typedef enum {
	DDENM_PLAYSTATE_STOP,
	DDENM_PLAYSTATE_PLAY,
	DDENM_PLAYSTATE_PAUSE
} emMediaPlayState;

class ddMediaPlayerPrivate;
class ddMediaPlayer {
	DD_PRIVATE_DECLARE(ddMediaPlayer)
public:
	class interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onMediaPlayer_progress(ddUInt32 current, ddUInt32 duration) {}
		virtual ddVoid onMediaPlayer_playState(emMediaPlayState state) {}
		virtual ddVoid onMediaPlayer_id3Info(ddpCChar pTitle, ddpCChar pArtist, ddpCChar pAlbum) {}
		virtual ddVoid onMediaPlayer_playEnd() {}
		virtual ddVoid onMediaPlayer_playError(ddpCChar pError) {}
		virtual ddVoid onMediaPlayer_playWarning(ddpCChar pWarning) {}
	};
	ddMediaPlayer(interface* pOwner);
	~ddMediaPlayer();
	ddVoid setOwner(interface* pOwner);
	ddVoid playFile(ddpCChar pFileName);
	ddVoid play();
	ddVoid pause();
	ddVoid seek(ddUInt32 pos);
};
#endif // dd_mediaplayer_h

