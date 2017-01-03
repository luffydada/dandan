/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_reg.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-01-03 15:02      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <regex.h>

#include "../dandan.h"

class ddRegPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddReg)
public:
	ddRegPrivate() :m_isComp(no), m_ret(0) {
		memset(&m_reg, 0, sizeof(m_reg));
	}

	~ddRegPrivate() {
		free();
	}

	ddBool setPattern(ddpCChar pPattern) {
		if ( pPattern && strlen(pPattern) ) {
			m_strPattern = pPattern;
			return yes;
		}
		return no;
	}

	ddBool comp() {
		if ( !m_isComp ) {
			m_ret = regcomp(&m_reg, m_strPattern.data(), REG_EXTENDED);
			if ( !m_ret ) {
				m_isComp = yes;
			}
		}
		return !m_ret;
	}

    ddBool exec(ddpCChar data) {
		if ( m_isComp && data && strlen(data)) {
			regmatch_t match[1] = {0};
			m_ret = regexec(&m_reg, data, 1, match, 0);
			return !m_ret;
		}
		return no;
	}

    ddVoid error(ddpChar buf, ddUInt16 size) {
		if ( buf && size ) {
			regerror(m_ret, &m_reg, buf, size);
		}
	}

    ddVoid free() {
		regfree(&m_reg);
	}

private:
	regex_t m_reg;
	std::string m_strPattern;
	ddBool m_isComp;
	ddInt m_ret;
};

///////////////////////////////////////////////////////////////////////////////
ddReg::ddReg(ddpCChar pPattern)
{
	DD_D_NEW(ddRegPrivate);
	dPtr()->setPattern(pPattern);
}

ddReg::~ddReg()
{
	DD_D_DELETE();
}

ddBool ddReg::comp()
{
	return dPtr()->comp();
}

ddBool ddReg::exec(ddpCChar data)
{
	return dPtr()->exec(data);
}

ddVoid ddReg::error(ddpChar buf, ddUInt16 size)
{
	dPtr()->error(buf, size);
}

ddVoid ddReg::free()
{
	dPtr()->free();
}

