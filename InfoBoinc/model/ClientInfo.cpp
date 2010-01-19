/*
 * =====================================================================
 *
 *       Filename:  ClientInfo.cpp
 *
 *        Version:  1.0
 *        Created:  19.01.2010 16:42:32
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "ClientInfo.h"

namespace InfoBoinc {

QMap<QString, int> ClientInfo::m_attributeTypes;

ClientInfo::ClientInfo()
{
}

ClientInfo::ClientInfo(const QDomElement &clientInfoNode)
{
	if (m_attributeTypes.count() == 0) {
		m_attributeTypes["platform_name"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["core_client_major_version"] = XMLAttributeMap::IntAttribute;
		m_attributeTypes["core_client_minor_version"] = XMLAttributeMap::IntAttribute;
		m_attributeTypes["core_client_release"] = XMLAttributeMap::IntAttribute;
		m_attributeTypes["platform"] = XMLAttributeMap::StringAttribute | XMLAttributeMap::ListAttribute;
	}
	m_attributes.parseAttributes(clientInfoNode, m_attributeTypes);
}

ClientInfo::~ClientInfo()
{
}

bool ClientInfo::isNull() const
{
	return m_attributes.isNull();
}

QString ClientInfo::platformName() const
{
	return m_attributes.attribute("platform_name").toString();
}

int ClientInfo::coreClientMajorVersion() const
{
	return m_attributes.attribute("core_client_major_version").toInt();
}

int ClientInfo::coreClientMinorVersion() const
{
	return m_attributes.attribute("core_client_minor_version").toInt();
}

int ClientInfo::coreClientRelease() const
{
	return m_attributes.attribute("core_client_release").toInt();
}

QString ClientInfo::coreClientVersion() const
{
	return QString("%1.%2.%3").arg(coreClientMajorVersion()).arg(coreClientMinorVersion()).arg(coreClientRelease());
}

QStringList ClientInfo::platform() const
{
	return m_attributes.attribute("platform").toStringList();
}

bool operator==(const ClientInfo &lhs, const ClientInfo &rhs)
{
	return lhs.m_attributes == rhs.m_attributes;
}

bool operator!=(const ClientInfo &lhs, const ClientInfo &rhs)
{
	return lhs.m_attributes != rhs.m_attributes;
}

} /* end of namespace InfoBoinc */

