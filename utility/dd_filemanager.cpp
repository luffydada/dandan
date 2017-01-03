/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_filemanager.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 13:51      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include "../dandan.h"

ddBool ddFileManager::isFileExist(ddpCChar path)
{
	if ( !path ) {
		return no;
	}
	return !access(path, F_OK);
}
 
ddBool ddFileManager::isDir(ddpCChar pPath)
{
	if ( isFileExist(pPath) ) {
		struct stat st = {0};
		lstat(pPath, &st);
		if (S_ISDIR(st.st_mode)) {
			return yes;
		}
	}
	return no;
}
ddBool ddFileManager::isFileReadable(ddpCChar path)
{
	if ( isFileExist(path) ) {
		return !access(path, R_OK);
	}
	return no;
}

ddBool ddFileManager::isFileWritable(ddpCChar path)
{
	if ( isFileExist(path) ) {
		return !access(path, W_OK);
	}
	return no;
}

ddBool ddFileManager::isFileExecutable(ddpCChar path)
{
	if ( isFileExist(path) ) {
		return !access(path, X_OK);
	}
	return no;
}

ddBool ddFileManager::getFileDirByPath(ddpCChar pPath, ddpChar pDir, ddUInt16 len)
{
	if ( pPath && pDir && len ) {
		ddpCChar pos = strrchr(pPath, '/');
		if ( pos ) {
			int length = strlen(pPath) - strlen(pos) + 1;
			if ( length < len ) {
				strncpy(pDir, pPath, length);
				*(pDir + length) = '\0';
				return yes;
			}
		}
	}
	return no;
}

ddBool ddFileManager::getFileNameByPath(ddpCChar pPath, ddpChar pName, ddUInt16 len)
{
	if ( pPath && pName && len ) {
		ddpCChar pos = strrchr(pPath, '/');
		if ( pos ) {
			int length = strlen(pos);
			if ( length < len ) {
				strcpy(pName, pos);
				*(pName + length) = '\0';
				return yes;
			}
		}
	}
	return no;
}

