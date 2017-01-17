/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_playlist.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-01-03 16:32      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <dirent.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include "../dandan.h"

ddpCChar MUSICFILTER[] = {
	"aac", "ac3", "arm", "ape", "m4a", "flac",  "mp3", "mp2", "ogg", "wav", "wma", "mid", "ra", nil
};

ddpCChar VIDEOFILTER[] = {
	"3gp", "swf", "avi", "mp4", "mkv", "mpg", "wmv", "mp3", "rm", "rmvb", "m4v", "mov", nil
};

ddpCChar PHOTOFILTER[] = {
	"bmp", "jpg", "tiff", "gif", "tga", "png", "jpeg", nil
};

class ddPlaylistPrivate : public ddPrivateBase, public ddThread::interface {
	DD_PUBLIC_DECLARE(ddPlaylist)
public:
	ddPlaylistPrivate() :m_pOwner(nil), m_findThread(this), m_isExit(no) {
		m_findThread.create();
	}

	~ddPlaylistPrivate() {
		m_isExit = yes;
		m_mutex.lock();
		m_cond.signal();
		m_mutex.unlock();
		cleanMediaData();
	}

	virtual ddVoid onThread(ddThread* pThread) {
		if ( pThread == &m_findThread ) {
			while ( !m_isExit ) {
				m_mutex.lock();
				m_cond.wait(&m_mutex);
				if ( m_pOwner ) {
					m_pOwner->onPlaylist_findStatus(DDENUM_FINDSTATUS_START);
				}
				enumFile(m_findPath.data());
				m_mutex.unlock();
				if ( m_pOwner ) {
					m_pOwner->onPlaylist_findStatus(DDENUM_FINDSTATUS_END);
				}
			}
		}
	}

	ddVoid setOwner(ddPlaylist::interface *pOwner) {
		m_pOwner = pOwner;
	}

	ddVoid startFind(ddpCChar pPath) {
		if ( pPath && strlen(pPath) ) {
			m_mutex.lock();
			m_findPath = pPath;
			cleanMediaData();
			m_cond.signal();
			m_mutex.unlock();
		}
	}

	ddVoid stopFind() {
	}

	ddBool enumFile(const char *pDirName) {
		if ( ddFileManager::isDir(pDirName) ) {
			DIR *pDir = ::opendir(pDirName);
			if ( pDir ) {
				struct dirent * pDirent = nil;
				do {
					pDirent = ::readdir(pDir);
					if ( pDirent ) {
						if ( !strncmp(".", pDirent->d_name, 1) || !strncmp("..", pDirent->d_name, 2) ) {
							continue;
						}
						ddChar path[1024] = {0};
						sprintf(path, "%s/%s", pDirName, pDirent->d_name);
//						printf("readdir,child file:%s\n", path);
						if ( ddFileManager::isFileExist(path) ) {
							if ( ddFileManager::isDir(path) ) {
								enumFile(path);
							} else {
								ddpChar pPos = strrchr(path, '.');
								if ( pPos && strlen(pPos) > 1 ) {
									ddBool isOkFile = no;
									ddUInt8 count = 0;
									while ( *(MUSICFILTER + count) ) {
										if ( !strcasecmp(pPos + 1, *(MUSICFILTER + count)) ) {
											isOkFile = yes;
											pstMediaInfo file = new stMediaInfo;
											file->type = DDENUM_MEDIATYPE_MUSIC;
											file->data.filePath = path;
											ddChar name[DD_MAXPATH] = {0};
											ddFileManager::getFileNameByPath(path, name, DD_MAXPATH);
											file->data.fileName = name;
//											printf("music push file:%s\n", file->data());
											TagLib::FileRef tag(path);
											if ( !tag.isNull() && tag.tag() ) {
												file->data.id3.title = tag.tag()->title().toCString(yes);
												file->data.id3.artist = tag.tag()->artist().toCString(yes);
												file->data.id3.album = tag.tag()->album().toCString(yes);
											}
											m_mediaVector.push_back(file);
											break;
										}
										++count;
									}
									if ( !isOkFile ) {
										count = 0;
										while ( *(VIDEOFILTER + count) ) {
											if ( !strcasecmp(pPos + 1, *(VIDEOFILTER + count)) ) {
												isOkFile = yes;
												pstMediaInfo file = new stMediaInfo;
												file->type = DDENUM_MEDIATYPE_VIDEO;
												file->data.filePath = path;
												ddChar name[DD_MAXPATH] = {0};
												ddFileManager::getFileNameByPath(path, name, DD_MAXPATH);
												file->data.fileName = name;
//												printf("video push file:%s\n", file->data());
												TagLib::FileRef tag(path);
												if ( !tag.isNull() && tag.tag() ) {
													file->data.id3.title = tag.tag()->title().toCString(yes);
													file->data.id3.artist = tag.tag()->artist().toCString(yes);
													file->data.id3.album = tag.tag()->album().toCString(yes);
												}
												m_mediaVector.push_back(file);
												break;
											}
											count++;
										}
									}
									if ( !isOkFile ) {
										count = 0;
										while ( *(PHOTOFILTER + count) ) {
											if ( !strcasecmp(pPos + 1, *(PHOTOFILTER + count)) ) {
												isOkFile = yes;
												pstMediaInfo file = new stMediaInfo;
												file->type = DDENUM_MEDIATYPE_PHOTO;
												file->data.filePath = path;
												ddChar name[DD_MAXPATH] = {0};
												ddFileManager::getFileNameByPath(path, name, DD_MAXPATH);
												file->data.fileName = name;
												TagLib::FileRef tag(path);
												if ( !tag.isNull() && tag.tag() ) {
													file->data.id3.title = tag.tag()->title().toCString(yes);
													file->data.id3.artist = tag.tag()->artist().toCString(yes);
													file->data.id3.album = tag.tag()->album().toCString(yes);
												}
												m_mediaVector.push_back(file);
//												printf("photo push file:%s\n", file->data());
												break;
											}
											count ++;
										}
									}
								}
							}
						}
					}
				} while ( pDirent );
				::closedir(pDir);
				return yes;
			} else {
				dd_log_d("opendir:%s,error:%s\n", pDirName, strerror(errno));
			}
		}
		return no;
	}

	ddVoid cleanMediaData() {
		std::vector<pstMediaInfo>::const_iterator it = m_mediaVector.begin();
		while ( it != m_mediaVector.end() ) {
			delete *it;
			it++;
		}
		m_mediaVector.clear();
	}

private:
	ddPlaylist::interface* m_pOwner;
	ddThread m_findThread;
	ddMutex m_mutex;
	ddCond m_cond;
	ddBool m_isExit;
	std::string m_findPath;
	std::vector<pstMediaInfo> m_mediaVector;
};

///////////////////////////////////////////////////////////////////////////////
ddPlaylist::ddPlaylist(interface* pOwner)
{
	DD_D_NEW(ddPlaylistPrivate);
	dPtr()->setOwner(pOwner);
}

ddPlaylist::~ddPlaylist()
{
	DD_D_DELETE();
}

ddVoid ddPlaylist::setOwner(interface *pOwner)
{
	dPtr()->setOwner(pOwner);
}

ddVoid ddPlaylist::startFind(ddpCChar pPath)
{
	dPtr()->startFind(pPath);
}

ddVoid ddPlaylist::stopFind()
{
	dPtr()->stopFind();
}

ddVoid ddPlaylist::requestMusicListByName()
{
}

ddVoid ddPlaylist::requestMusicListByArtist()
{
}

ddVoid ddPlaylist::requestMusicListByAlbum()
{
}

ddVoid ddPlaylist::requestVideoList()
{
}

ddVoid ddPlaylist::requestPhotoList()
{
}

