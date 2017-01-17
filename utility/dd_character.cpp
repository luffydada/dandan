/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_character.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-01-10 10:56      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <iconv.h>

#include "../dandan.h"

ddBool ddCharacter::utf8ToUtf16(ddpUtf8 pin, ddpUtf16 pout, ddUInt16 uout)
{
	return codeConvert("UTF-8", "UTF-16", pin, (ddpChar)pout, uout*2);
}

ddBool ddCharacter::utf16ToUtf8(ddpUtf16 pin, ddpUtf8 pout, ddUInt16 uout)
{
	return codeConvert("UTF-8", "UTF-16", (ddpChar)pin, pout, uout);
}

ddBool ddCharacter::codeConvert(ddpCChar toCode, ddpCChar fromCode, ddpChar pin, ddpChar pout, ddUInt16 uout)
{
	if ( toCode && fromCode && pin && pout && uout ) {
		iconv_t icd = iconv_open(toCode, fromCode);
		if ( (iconv_t)-1 != icd ) {
			size_t len = strlen(pin);
			memset(pout, 0, uout);
			ddBool isOk = no;
			if ( -1 != iconv(icd, &pin, &len, &pout, (size_t *)&uout) ) {
				isOk = yes;
			}
			iconv_close(icd);
			icd = (iconv_t)-1;
			return isOk;
		}
	}
	return no;
}

