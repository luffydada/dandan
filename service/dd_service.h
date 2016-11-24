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
	static ddInt execsh(ddpCChar cmd, std::vector<std::string> &resvec);
	static ddVoid startupServer();
	static ddVoid startup(ddpCChar pName, ddBool isServer = no);
	static ddVoid ioctl(ddpCByte pin, ddUInt16 uin, ddpByte pout = nil, ddUInt16 uout = 0);
};

#endif // dd_service_h
