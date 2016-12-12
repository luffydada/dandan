/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_log.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 16:44      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_log_h
#define dd_log_h
class ddLog {
public:	
	static ddVoid d(ddpCChar fmt, ...);
	static ddVoid i(ddpCChar fmt, ...);
	static ddVoid w(ddpCChar fmt, ...);
	static ddVoid e(ddpCChar fmt, ...);
	static ddVoid log(ddUInt8 type, ddpCChar file, ddUInt16 line, ddpCChar fmt, ...);
};

#define dd_log_d(fmt, ...) ddLog::log(0, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define dd_log_i(fmt, ...) ddLog::log(1, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define dd_log_w(fmt, ...) ddLog::log(2, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define dd_log_e(fmt, ...) ddLog::log(3, __FILE__, __LINE__, fmt, __VA_ARGS__)

#define dd_log_d2(fmt) ddLog::log(0, __FILE__, __LINE__, fmt)
#define dd_log_i2(fmt) ddLog::log(1, __FILE__, __LINE__, fmt)
#define dd_log_w2(fmt) ddLog::log(2, __FILE__, __LINE__, fmt)
#define dd_log_e2(fmt) ddLog::log(3, __FILE__, __LINE__, fmt)
#endif // dd_log_h

