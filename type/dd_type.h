/***************************************************************************
*    File name:        dd_type.h
*    Description:
*    Create date:      2016-11-20 16:04
*    Modified date:    2016-11-21 22:21
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#ifndef __dd_type_h__
#define __dd_type_h__

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 常用类型定义
 */
/* --------------------------------------------------------------------------*/
typedef void					ddVoid;
typedef char					ddChar;
typedef unsigned char			ddUChar;
typedef wchar_t					ddWChar;
typedef unsigned char			ddByte;
typedef unsigned char			ddBool;
typedef char					ddInt8;
typedef unsigned char			ddUInt8;
typedef short					ddInt16;
typedef unsigned short			ddUInt16;
typedef short					ddShort;
typedef unsigned short			ddUShort;
typedef int						ddInt;
typedef unsigned int			ddUInt;
typedef int						ddInt32;
typedef unsigned int			ddUInt32;
typedef long long				ddInt64;
typedef unsigned long long		ddUInt64;
typedef long					ddLong;
typedef unsigned long			ddULong;
typedef float					ddFloat;
typedef double					ddDouble;

typedef ddVoid *				ddPointer;
typedef ddChar *				ddpChar;
typedef const ddChar *			ddpCChar;
typedef ddUChar *				ddpUChar;
typedef const ddUChar *			ddpCUChar;
typedef ddWChar *				ddpWChar;
typedef const ddWChar *			ddpCWChar;
typedef ddByte *				ddpByte;
typedef const ddByte *			ddpCByte;
typedef ddBool *				ddpBool;
typedef const ddBool *			ddpCBool;
typedef ddInt *					ddpInt;
typedef const ddInt *			ddpCInt;
typedef ddUint *				ddpUint;
typedef const ddUint *			ddpCUint;
typedef ddShort *				ddpShort;
typedef const ddShort *			ddpCShort;
typedef ddLong *				ddpLong;
typedef const ddLong *			ddpCLong;
typedef ddFloat *				ddpFloat;
typedef const ddFloat *			ddpCFloat;
typedef ddDouble *				ddpDouble;
typedef const ddDouble *		ddpCDouble;

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 常用宏类型定义
 */
/* --------------------------------------------------------------------------*/
#define dd_msg
#define nil				(0L)
#define yes				(1)
#define no				(0)

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 函数统一宏定义
 */
/* --------------------------------------------------------------------------*/
#define stricmp strcasecmp

#endif //dd_type.h
