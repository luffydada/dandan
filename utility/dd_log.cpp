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

ddpCChar COLOR[DDENUM_PRINTCOLOR_TOTAL] = {
	"\033[0m",					///< font none
	"\033[1;30m",				///< font black
   	"\033[1;31m",				///< font red
   	"\033[1;32m",				///< font green
	"\033[1;33m",				///< font yellow
   	"\033[1;34m",				///< font blue
	"\033[1;35m",				///< font purple
	"\033[1;36m",				///< font cyan
	"\033[1;37m",				///< font white
	"\033[1;40m",				///< bg black 
   	"\033[1;41m",				///< bg red
   	"\033[1;42m",				///< bg green
	"\033[1;43m",				///< bg yellow
   	"\033[1;44m",				///< bg blue
	"\033[1;45m",				///< bg purple
	"\033[1;46m",				///< bg cyan
	"\033[1;47m",				///< bg white
};

ddVoid ddLog::color_print(emPrintColor color, ddpCChar fmt)
{
	if ( color < DDENUM_PRINTCOLOR_TOTAL ) {
		printf("%s%s%s", COLOR[color], fmt, COLOR[DDENUM_PRINTCOLOR_NONE]);
	} else {
		printf("%s", fmt);
	}
}

ddVoid ddLog::d(ddpCChar fmt, ...)
{
#ifdef __DD_DEBUG__
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	color_print(DDENUM_PRINTCOLOR_NONE, buffer);
#endif
}

ddVoid ddLog::i(ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	color_print(DDENUM_PRINTCOLOR_FONT_GREEN, buffer);
}

ddVoid ddLog::w(ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	color_print(DDENUM_PRINTCOLOR_FONT_YELLOW, buffer);
}

ddVoid ddLog::e(ddpCChar fmt, ...)
{
	ddChar buffer[1024] = {0};
	va_list p;
	va_start(p, fmt);
	vsprintf(buffer, fmt, p);
	va_end(p);
	color_print(DDENUM_PRINTCOLOR_FONT_RED, buffer);
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

