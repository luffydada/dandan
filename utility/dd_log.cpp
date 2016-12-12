/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_log.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-12 16:44      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <stdarg.h>
#include "../dandan.h"

ddVoid ddLog::d(ddpCChar fmt, ...)
{
#ifdef __DD_DEBUG__
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	printf("%s", buffer);
#endif
}

ddVoid ddLog::i(ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	printf("%s", buffer);
}

ddVoid ddLog::w(ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	printf("%s", buffer);
}

ddVoid ddLog::e(ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	printf("%s", buffer);
}

ddVoid ddLog::log(ddUInt8 type, ddpCChar file, ddUInt16 line, ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	sprintf(buffer, "%s:%d->", file, line);
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer + strlen(buffer), fmt, p);
	va_end(p);
	if ( 0 == type ) {
		d(buffer);
	} else if ( 1 == type ){
		i(buffer);
	} else if ( 2 == type ){
		w(buffer);
	} else if ( 3 == type ){
		e(buffer);
	}
}

