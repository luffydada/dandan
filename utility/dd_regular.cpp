/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_regular.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2017-05-15 14:03      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <regex.h>

#include "../dandan.h"

class ddRegularPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddRegular)
public:
	ddRegularPrivate() :m_isComp(no), m_ret(0) {
		memset(&m_regular, 0, sizeof(m_regular));
	}

	~ddRegularPrivate() {
		free();
	}

	ddBool setPattern(ddpCChar pPattern) {
		if ( pPattern && strlen(pPattern) ) {
			m_strPattern = pPattern;
			return yes;
		}
		return no;
	}

	ddBool compile() {
		if ( !m_isComp ) {
			m_ret = regcomp(&m_regular, m_strPattern.data(), REG_EXTENDED);
			if ( !m_ret ) {
				m_isComp = yes;
			}
		}
		return !m_ret;
	}

    ddBool exec(ddpCChar data) {
		if ( m_isComp && data && strlen(data)) {
			regmatch_t match[1] = {0};
			m_ret = regexec(&m_regular, data, 1, match, 0);
			return !m_ret;
		}
		return no;
	}

    ddVoid error(ddpChar buf, ddUInt16 size) {
		if ( buf && size ) {
			regerror(m_ret, &m_regular, buf, size);
		}
	}

    ddVoid free() {
		regfree(&m_regular);
	}

private:
	regex_t m_regular;
	std::string m_strPattern;
	ddBool m_isComp;
	ddInt m_ret;
};

///////////////////////////////////////////////////////////////////////////////
ddRegular::ddRegular(ddpCChar pPattern)
{
	DD_D_NEW(ddRegularPrivate);
	dPtr()->setPattern(pPattern);
}

ddRegular::~ddRegular()
{
	DD_D_DELETE();
}

ddBool ddRegular::compile()
{
	return dPtr()->compile();
}

ddBool ddRegular::exec(ddpCChar data)
{
	return dPtr()->exec(data);
}

ddVoid ddRegular::error(ddpChar buf, ddUInt16 size)
{
	dPtr()->error(buf, size);
}

ddVoid ddRegular::free()
{
	dPtr()->free();
}

