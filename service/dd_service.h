/***************************************************************************
*    File name:        dd_service.h
*    Description:
*    Create date:      2016-11-23 21:45
*    Modified date:    2016-11-23 21:45
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#ifndef dd_service_h
#define dd_service_h

class ddServicePrivate;
class ddService {
	DD_PRIVATE_DECLARE(ddService)
public:
	ddService();
	~ddService();
	ddInt ddService::execsh(ddpCChar cmd, std::vector<std::string> &resvec)
	ddVoid startup(ddpCChar pName, ddBool isServer = no);
};

#endif // dd_service_h
