/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_character.h
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-01-10 10:55      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#ifndef dd_character_h
#define dd_character_h

class ddCharacter {
public:	
	static ddBool utf8ToUtf16(ddpUtf8 pin, ddpUtf16 pout, ddUInt16 uout);
	static ddBool utf16ToUtf8(ddpUtf16 pin, ddpUtf8 pout, ddUInt16 uout);
	static ddBool codeConvert(ddpCChar toCode, ddpCChar fromCode, ddpChar pin, ddpChar pout, ddUInt16 uout);
};
#endif // dd_character_h

