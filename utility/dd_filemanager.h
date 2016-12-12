/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_filemanager.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 13:51      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_filemanager_h
#define dd_filemanager_h
class ddFileManager {
public:	
	static ddBool isFileExist(ddpCChar path);
	static ddBool isFileReadable(ddpCChar path);
	static ddBool isFileWritable(ddpCChar path);
	static ddBool isFileExecutable(ddpCChar path);

	static ddBool getFileDirByPath(ddpCChar pPath, ddpChar pDir, ddUInt16 len);
	static ddBool getFileNameByPath(ddpCChar pPath, ddpChar pName, ddUInt16 len);
};

#endif // dd_filemanager_h

