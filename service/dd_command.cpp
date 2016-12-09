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

ddCommand::interface::~interface() {}
ddVoid ddCommand::interface::onCommit(ddpCByte data, ddUInt16 len) {}
ddVoid ddCommand::interface::onDownload(ddpCByte data, ddUInt16 len) {}

ddCommand::ddCommand(interface *pOwner, ddUInt16 cmd/* = 0*/, emCommandType type/* = DDENUM_COMMAND_SERVICE*/, ddpCByte data/* = nil*/, ddUInt16 len/* = 0*/)
{
	m_pOwner = pOwner;
	m_len = 0;
	memset(m_data, 0, DD_MAXDATA);
	setType(type);
	setCommand(cmd);
	setData(data, len);
}

ddCommand::ddCommand(ddpCByte data, ddUInt16 len)
{
	m_pOwner = nil;
	m_len = 0;
	memset(m_data, 0, DD_MAXDATA);
	if ( data && len > 3 ) {
		setType(emCommandType(*data));
		setCommand(*(reinterpret_cast<const ddUInt16 *>(data + 1)));
		setData(data + 3, len - 3);
	}
}

ddCommand::~ddCommand()
{
}

ddVoid ddCommand::setCommand(ddUInt16 cmd)
{
	*(reinterpret_cast<ddUInt16 *>(m_data + 1)) = cmd;
}

ddUInt16 ddCommand::command()
{
	return *(reinterpret_cast<ddUInt16 *>(m_data + 1));
}

ddVoid ddCommand::setType(emCommandType type)
{
	m_data[0] = ddByte(type);
}

emCommandType ddCommand::type()
{
	return emCommandType(m_data[0]);
}

ddVoid ddCommand::setData(ddpCByte data, ddUInt16 len, ddUInt16 pos/* = 0*/)
{
	if ( data && (pos + len) <= maxLength() ) {
		memcpy(m_data + 3 + pos, data, len);
		m_len = m_len < (pos + len) ? (pos + len) : m_len;
	}
}

ddpCByte ddCommand::data()
{
	return m_data + 3;
}

ddUInt16 ddCommand::length()
{
	return m_len;
}

ddUInt16 ddCommand::maxLength()
{
	return DD_MAXDATA - 3;
}

ddVoid ddCommand::download()
{
	if ( m_pOwner ) {
		m_pOwner->onDownload(m_data, m_len + 3);
	}
}

ddVoid ddCommand::commit()
{
	if ( m_pOwner ) {
		m_pOwner->onCommit(m_data, m_len + 3);
	}
}

