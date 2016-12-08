/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_command.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-08 17:40      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include "../dandan.h"

class ddCommandPrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddCommand)
public:
	ddCommandPrivate() : m_pOwner(nil), m_len(0) {
		memset(m_data, 0, DD_MAXPATH);
	}

	~ddCommandPrivate() {
	}

	ddVoid setOwner(ddCommand::interface *pOwner) {
		m_pOwner = pOwner;
	}

	ddVoid setData(ddpCByte data, ddUInt16 len, ddUInt16 pos) {
		if ( data && (pos + len) <= maxLength() ) {
			memcpy(m_data + pos, data, len);
			m_len = m_len < (pos + len) ? (pos + len) : m_len;
		}
	}

	ddpCByte data() {
		return m_data;
	}

	ddUInt16 length() {
		return m_len;
	}

	ddUInt16 maxLength() {
		return DD_MAXDATA;
	}

	ddVoid download() {
		if ( m_pOwner ) {
			m_pOwner->onDownload(m_data, m_len);
		}
	}

	ddVoid commit() {
		if ( m_pOwner ) {
			m_pOwner->onCommit(m_data, m_len);
		}
	}

private:
	ddCommand::interface *m_pOwner;
	ddByte m_data[DD_MAXDATA];
	ddUInt16 m_len;
};

///////////////////////////////////////////////////////////////////////////////
ddCommand::ddCommand(interface *pOwner)
{
	DD_D_NEW(ddCommandPrivate);
	dPtr()->setOwner(pOwner);
}

ddCommand::~ddCommand()
{
	DD_D_DELETE();
}

ddVoid ddCommand::setData(ddpCByte data, ddUInt16 len, ddUInt16 pos/* = 0*/)
{
	dPtr()->setData(data, len, pos);
}

ddpCByte ddCommand::data()
{
	return dPtr()->data();
}

ddUInt16 ddCommand::length()
{
	return dPtr()->length();
}

ddUInt16 ddCommand::maxLength()
{
	return dPtr()->maxLength();
}

ddVoid ddCommand::download()
{
	dPtr()->download();
}

ddVoid ddCommand::commit()
{
	dPtr()->commit();
}

