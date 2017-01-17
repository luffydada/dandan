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

typedef enum {
	DDENUM_MEDIATYPE_UNKNOWN,
	DDENUM_MEDIATYPE_MUSIC,
	DDENUM_MEDIATYPE_VIDEO,
	DDENUM_MEDIATYPE_PHOTO
} emMediaType;

typedef struct {
	std::string title;
	std::string artist;
	std::string album;
} stMediaID3Info, *pstMediaID3Info;

typedef struct {
	std::string filePath;
	std::string fileName;
	stMediaID3Info id3;
} stMediaMetaData, *pstMediaMetaData;

typedef struct {
	emMediaType type;
	stMediaMetaData data;
} stMediaInfo, *pstMediaInfo;

class ddPlaylistPrivate;
class ddPlaylist {
	DD_PRIVATE_DECLARE(ddPlaylist)
public:
	class interface {
	public:
		virtual ~interface() {}
		virtual ddVoid onPlaylist_findStatus(emPlaylistFindStatus status) {}
		virtual ddVoid onPlaylist_musicList(ddUInt16 index, ddUInt16 total, pstMediaMetaData data) {}
		virtual ddVoid onPlaylist_videoList(ddUInt16 index, ddUInt16 total, pstMediaMetaData data) {}
		virtual ddVoid onPlaylist_photoList(ddUInt16 index, ddUInt16 total, pstMediaMetaData data) {}
	};
	ddPlaylist(interface *pOwner);
	~ddPlaylist();
	ddVoid setOwner(interface *pOwner);
	ddVoid startFind(ddpCChar pPath);
	ddVoid stopFind();
	ddVoid requestMusicListByName();
	ddVoid requestMusicListByArtist();
	ddVoid requestMusicListByAlbum();
	ddVoid requestVideoList();
	ddVoid requestPhotoList();
};
#endif // dd_playlist_h

