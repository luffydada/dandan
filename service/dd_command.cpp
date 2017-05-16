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

ddCommand::ddCommand(interface *pOwner, ddUInt16 cmd/* = 0*/, emCommandType type/* = DDENUM_COMMAND_SERVICE*/, ddUInt16 len/* = 0*/, ddpCByte data/* = nil*/)
{
	m_pOwner = pOwner;
	m_len = 0;
	memset(m_data, 0, DD_MAXDATA);
	setType(type);
	command() = cmd;
	len = len > maxLength() ? maxLength() : len;
	length() = len;
	if ( data ) {
		memcpy(this->data(), data, len);
	}
}

ddCommand::ddCommand(ddpCByte data, ddUInt16 len)
{
	m_pOwner = nil;
	m_len = 0;
	memset(m_data, 0, DD_MAXDATA);
	if ( data && len > 3 ) {
		setType(emCommandType(*data));
		command() = *(reinterpret_cast<const ddUInt16 *>(data + 1));
		len = len > (maxLength() + 3) ? (maxLength() + 3) : len;
		length() = len - 3;
		memcpy(this->data(), data + 3, len - 3);
	}
}

ddCommand::~ddCommand()
{
}

ddUInt16& ddCommand::command()
{
	return *(reinterpret_cast<ddUInt16 *>(m_data + 1));
}

emCommandType ddCommand::type()
{
	return emCommandType(m_data[0]);
}

emCommandType ddCommand::setType(emCommandType type)
{
	m_data[0] = ddByte(type);
	return type;
}

ddByte& ddCommand::operator[](ddUInt16 pos)
{
	if ( pos >= maxLength() ) {
		pos = 0;
	}
	return m_data[3 + pos];
}

ddpByte ddCommand::data()
{
	return m_data + 3;
}

ddUInt16& ddCommand::length()
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

