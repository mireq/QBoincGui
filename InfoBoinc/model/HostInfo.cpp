/*
 * =====================================================================
 *
 *       Filename:  HostInfo.cpp
 *
 *        Version:  1.0
 *        Created:  18.01.2010 22:28:04
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "HostInfo.h"

namespace InfoBoinc {

QMap<QString, int> HostInfo::m_attributeTypes;

HostInfo::HostInfo()
{
}

HostInfo::HostInfo(const QDomElement &hostInfoNode)
{
	if (m_attributeTypes.count() == 0) {
		m_attributeTypes["timezone"] = XMLAttributeMap::IntAttribute;
		m_attributeTypes["domain_name"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["ip_addr"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["p_ncpus"] = XMLAttributeMap::IntAttribute;
		m_attributeTypes["p_vendor"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["p_model"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["p_features"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["p_fpops"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["p_iops"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["m_nbytes"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["m_cache"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["m_swap"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["d_total"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["d_free"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["os_name"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["os_version"] = XMLAttributeMap::StringAttribute;
	}
	m_attributes.parseAttributes(hostInfoNode, m_attributeTypes);
}

HostInfo::~HostInfo()
{
}

bool HostInfo::isNull() const
{
	return m_attributes.isNull();
}

int HostInfo::timezone() const
{
	return m_attributes.attribute("timezone").toInt();
}

QString HostInfo::domainName() const
{
	return m_attributes.attribute("domain_name").toString();
}

QString HostInfo::ipAddr() const
{
	return m_attributes.attribute("ip_addr").toString();
}

int HostInfo::pCpus() const
{
	return m_attributes.attribute("p_ncpus").toInt();
}

QString HostInfo::pVendor() const
{
	return m_attributes.attribute("p_vendor").toString();
}

QString HostInfo::pModel() const
{
	return m_attributes.attribute("p_model").toString();
}

QString HostInfo::pFeatures() const
{
	return m_attributes.attribute("p_features").toString();
}

double HostInfo::pFpops() const
{
	return m_attributes.attribute("p_fpops").toDouble();
}

double HostInfo::pIops() const
{
	return m_attributes.attribute("p_iops").toDouble();
}

double HostInfo::mNbytes() const
{
	return m_attributes.attribute("m_nbytes").toDouble();
}

double HostInfo::mCache() const
{
	return m_attributes.attribute("m_cache").toDouble();
}

double HostInfo::mSwap() const
{
	return m_attributes.attribute("m_swap").toDouble();
}

double HostInfo::dTotal() const
{
	return m_attributes.attribute("d_total").toDouble();
}

double HostInfo::dFree() const
{
	return m_attributes.attribute("d_free").toDouble();
}

QString HostInfo::osName() const
{
	return m_attributes.attribute("os_name").toString();
}

QString HostInfo::osVersion() const
{
	return m_attributes.attribute("os_version").toString();
}

bool operator==(const HostInfo &lhs, const HostInfo &rhs)
{
	return lhs.m_attributes == rhs.m_attributes;
}

bool operator!=(const HostInfo &lhs, const HostInfo &rhs)
{
	return lhs.m_attributes != rhs.m_attributes;
}

} /* end of namespace InfoBoinc */

