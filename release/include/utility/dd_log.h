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

typedef enum {
	DDENUM_PRINTCOLOR_NONE,
	DDENUM_PRINTCOLOR_FONT_BLACK,
	DDENUM_PRINTCOLOR_FONT_RED,
	DDENUM_PRINTCOLOR_FONT_GREEN,
	DDENUM_PRINTCOLOR_FONT_YELLOW,
	DDENUM_PRINTCOLOR_FONT_BLUE,
	DDENUM_PRINTCOLOR_FONT_PURPLE,
	DDENUM_PRINTCOLOR_FONT_CYAN,
	DDENUM_PRINTCOLOR_FONT_WHITE,
	DDENUM_PRINTCOLOR_BG_BLACK,
	DDENUM_PRINTCOLOR_BG_RED,
	DDENUM_PRINTCOLOR_BG_GREEN,
	DDENUM_PRINTCOLOR_BG_YELLOW,
	DDENUM_PRINTCOLOR_BG_BLUE,
	DDENUM_PRINTCOLOR_BG_PURPLE,
	DDENUM_PRINTCOLOR_BG_CYAN,
	DDENUM_PRINTCOLOR_BG_WHITE,
	DDENUM_PRINTCOLOR_TOTAL,
}emPrintColor;

class ddLog {
public:	
	static ddVoid color_print(emPrintColor color, ddpCChar fmt);
	static ddVoid d(ddpCChar fmt, ...);
	static ddVoid i(ddpCChar fmt, ...);
	static ddVoid w(ddpCChar fmt, ...);
	static ddVoid e(ddpCChar fmt, ...);
	static ddVoid log(ddUInt8 type, ddpCChar file, ddUInt16 line, ddpCChar fmt, ...);
};

#define dd_log_d(fmt, ...) ddLog::log(0, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define dd_log_i(fmt, ...) ddLog::log(1, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define dd_log_w(fmt, ...) ddLog::log(2, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define dd_log_e(fmt, ...) ddLog::log(3, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif // dd_log_h
