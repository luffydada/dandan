/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_process.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 11:49      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../dandan.h"

class ddProcessPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddProcess)
public:
	ddProcessPrivate() {
		struct sigaction act;
		act.sa_handler = sig_handler;
		act.sa_flags = 0;
		sigemptyset(&(act.sa_mask));
		sigaction(SIGCHLD, &act, nil);
	}

	~ddProcessPrivate() {
	}

	static ddVoid sig_handler(int sigId) {
		if ( ddProcessPrivate *pInst = ddGlobalInstance<ddProcessPrivate>::instance(yes) ) {
			std::list<pid_t>::const_iterator it = pInst->m_pidList.begin();
			int status = 0;
			while ( it != pInst->m_pidList.end() ) {
				if ( *it == waitpid(*it, &status, WNOHANG) ) {
					printf("sig_handler,process exit,pid:%d\n", *it);
				}
				it++;
			}
		}
	}

	ddVoid add(pid_t pid) {
		ddBool isExist = no;
		std::list<pid_t>::const_iterator it = m_pidList.begin();
		while ( it != m_pidList.end() ) {
			if ( pid == *it ) {
				isExist = yes;
				break;
			}
			it++;
		}
		if ( !isExist ) {
			m_pidList.push_back(pid);
		}
	}

private:
	std::list<pid_t> m_pidList;
};

///////////////////////////////////////////////////////////////////////////////
ddProcess::ddProcess()
{
	DD_D_NEW(ddProcessPrivate);
}

ddProcess::~ddProcess()
{
	DD_D_DELETE();
}

ddBool ddProcess::createProcess(ddpCChar path, ddpCChar cmd[])
{
	if ( ddFileManager::isFileExecutable(path) ) {
		pid_t pid = fork();
		if ( pid < 0 ) {///< fork failed;
			printf("ddProcess,createProcess,fork child process error:%d,path:%s\n", errno, path);
		} else if ( pid > 0 ) {///< parent process;
			if ( ddProcessPrivate *pInst = ddGlobalInstance<ddProcessPrivate>::instance(yes) ) {
				pInst->add(pid);
			}
			return yes;
		} else {///< child process
			execv(path, (ddpChar *)cmd);
			printf("ddProcess,createProcess,execv error:%d,path:%s\n", errno, path);
		}
	}
	return no;
}

ddBool ddProcess::getModuleName(ddpChar path, ddUInt16 len)
{
	if ( path && len ) {
		return (-1 != readlink("/proc/self/exe", path, len));
	}
	return no;
}

ddBool ddProcess::getModulePath(ddpChar path, ddUInt16 len)
{
	ddChar moduleName[DD_MAXPATH] = {0};
	return ( getModuleName(moduleName, DD_MAXPATH) && ddFileManager::getFileDirByPath(moduleName, path, len) );
}

