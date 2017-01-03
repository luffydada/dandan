/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_playlist.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-01-03 16:32      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_playlist_h
#define dd_playlist_h

typedef enum {
	DDENUM_FINDSTATUS_START,
	DDENUM_FINDSTATUS_END
} emPlaylistFindStatus;

class ddPlaylistPrivate;
class ddPlaylist {
	DD_PRIVATE_DECLARE(ddPlaylist)
public:
	class interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onPlaylist_findStatus(emPlaylistFindStatus status) {}
	};
	ddPlaylist(interface *pOwner);
	~ddPlaylist();
	ddVoid setOwner(interface *pOwner);
	ddVoid startFind(ddpCChar pPath);
	ddVoid stopFind();
};
#endif // dd_playlist_h

