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

#include "../dandan.h"

const ddChar MUSICFILTER[][10] = {
	"aac", "ac3", "arm", "ape", "m4a", "flac",  "mp3", "mp2", "ogg", "wav", "wma", "mid", "ra"
};

const ddChar VIDEOFILTER[][10] = {
	"3gp", "swf", "avi", "mp4", "mkv", "mpg", "wmv", "mp3", "rm", "rmvb", "m4v", "mov"
};

const ddChar PHOTOFILTER[][10] = {
	"bmp", "jpg", "tiff", "gif", "tga", "png", "jpeg"
};

class ddPlaylistPrivate : public ddPrivateBase, public ddThread::interface {
	DD_PUBLIC_DECLARE(ddPlaylist)
public:
	ddPlaylistPrivate() :m_pOwner(nil), m_findThread(this), m_isExit(no) {
	}

	~ddPlaylistPrivate() {
		m_isExit = yes;
		m_mutex.lock();
		m_cond.signal();
		m_mutex.unlock();
		cleanMusicData();
		cleanVideoData();
		cleanPhotoData();
	}

	virtual ddVoid onThread(ddThread* pThread) {
		if ( pThread == &m_findThread ) {
			m_mutex.lock();
			m_cond.wait(&m_mutex);
			while ( !m_isExit ) {
				if ( m_pOwner ) {
					m_pOwner->onPlaylist_findStatus(DDENUM_FINDSTATUS_START);
				}
				enumFile(m_findPath.data());
				if ( m_pOwner ) {
					m_pOwner->onPlaylist_findStatus(DDENUM_FINDSTATUS_END);
				}
			}
			m_mutex.unlock();
		}
	}

	ddVoid setOwner(ddPlaylist::interface *pOwner) {
		m_pOwner = pOwner;
	}

	ddVoid startFind(ddpCChar pPath) {
		if ( pPath && strlen(pPath) ) {
			m_mutex.lock();
			m_findPath = pPath;
			cleanMusicData();
			cleanVideoData();
			cleanPhotoData();
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
									for ( int i = 0; i < sizeof(MUSICFILTER) / 10; i++ ) {
										if ( !strcasecmp(pPos + 1, MUSICFILTER[i]) ) {
											std::string *file = new std::string(path);
											m_musicVector.push_back(file);
											isOkFile = yes;
//											printf("music push file:%s\n", file->data());
/*											TagLib::FileRef *pTag = new TagLib::FileRef(path);
											if ( !pTag->isNull() && pTag->tag() ) {
												g_tagVector.push_back(pTag);
											}*/
										}
									}
									if ( !isOkFile ) {
										for ( int i = 0; i < sizeof(VIDEOFILTER) / 10; i++ ) {
											if ( !strcasecmp(pPos + 1, VIDEOFILTER[i]) ) {
												std::string *file = new std::string(path);
												m_videoVector.push_back(file);
												isOkFile = yes;
//												printf("video push file:%s\n", file->data());
/*												TagLib::FileRef *pTag = new TagLib::FileRef(path);
												if ( !pTag->isNull() && pTag->tag() ) {
													g_tagVector.push_back(pTag);
												}*/
											}
										}
									}
									if ( !isOkFile ) {
										for ( int i = 0; i < sizeof(PHOTOFILTER) / 10; i++ ) {
											if ( !strcasecmp(pPos + 1, PHOTOFILTER[i]) ) {
												std::string *file = new std::string(path);
												m_photoVector.push_back(file);
												isOkFile = yes;
//												printf("photo push file:%s\n", file->data());
/*												TagLib::FileRef *pTag = new TagLib::FileRef(path);
												if ( !pTag->isNull() && pTag->tag() ) {
													g_tagVector.push_back(pTag);
												}*/
											}
										}
									}
								}
							}
						}
					}
				} while(pDirent);
				::closedir(pDir);
				return yes;
			} else {
				printf("opendir:%s,error:%s\n", pDirName, strerror(errno));
			}
		}
		return no;
	}

	ddVoid cleanMusicData() {
		std::vector<std::string *>::const_iterator it = m_musicVector.begin();
		while ( it != m_musicVector.end() ) {
			delete *it;
			it++;
		}
	}

	ddVoid cleanVideoData() {
		std::vector<std::string *>::const_iterator it = m_videoVector.begin();
		while ( it != m_videoVector.end() ) {
			delete *it;
			it++;
		}
	}

	ddVoid cleanPhotoData() {
		std::vector<std::string *>::const_iterator it = m_photoVector.begin();
		while ( it != m_photoVector.end() ) {
			delete *it;
			it++;
		}
	}
private:
	ddPlaylist::interface* m_pOwner;
	ddThread m_findThread;
	ddMutex m_mutex;
	ddCond m_cond;
	ddBool m_isExit;
	std::string m_findPath;
	std::vector<std::string *> m_musicVector;
	std::vector<std::string *> m_videoVector;
	std::vector<std::string *> m_photoVector;
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

