/*
 * =====================================================================
 *
 *       Filename:  WorkunitInfo.cpp
 *
 *        Version:  1.0
 *        Created:  22.01.2010 05:58:05
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "WorkunitInfo.h"

namespace InfoBoinc {

QMap<QString, int> WorkunitInfo::m_workunitAttributeTypes;

WorkunitInfo::WorkunitInfo()
{
}

WorkunitInfo::WorkunitInfo(const QDomElement &workunitInfoNode)
{
	if (m_workunitAttributeTypes.count() == 0) {
		m_workunitAttributeTypes["name"] = XMLAttributeMap::StringAttribute;
		m_workunitAttributeTypes["app_name"] = XMLAttributeMap::StringAttribute;
		m_workunitAttributeTypes["version_num"] = XMLAttributeMap::StringAttribute;
		m_workunitAttributeTypes["rsc_fpops_est"] = XMLAttributeMap::DoubleAttribute;
		m_workunitAttributeTypes["rsc_fpops_bound"] = XMLAttributeMap::DoubleAttribute;
		m_workunitAttributeTypes["rsc_memory_bound"] = XMLAttributeMap::DoubleAttribute;
		m_workunitAttributeTypes["rsc_disk_bound"] = XMLAttributeMap::DoubleAttribute;
	}
	m_attributes.parseAttributes(workunitInfoNode, m_workunitAttributeTypes);
	qDebug() << m_attributes;
}

WorkunitInfo::~WorkunitInfo()
{
}

bool WorkunitInfo::isNull() const
{
	return m_attributes.isNull();
}

QString WorkunitInfo::primaryKey() const
{
	return m_attributes.attribute("name").toString();
}

QString WorkunitInfo::name() const
{
	return m_attributes.attribute("name").toString();
}

QString WorkunitInfo::appnName() const
{
	return m_attributes.attribute("app_name").toString();
}

QString WorkunitInfo::versionNum() const
{
	return m_attributes.attribute("version_num").toString();
}

double WorkunitInfo::rscFpopsEst() const
{
	return m_attributes.attribute("rsc_fpops_est").toDouble();
}

double WorkunitInfo::rscFpopsBound() const
{
	return m_attributes.attribute("rsc_fpops_bound").toDouble();
}

double WorkunitInfo::rscMemoryBound() const
{
	return m_attributes.attribute("rsc_memory_bound").toDouble();
}

double WorkunitInfo::rscDiskBound() const
{
	return m_attributes.attribute("rsc_disk_bound").toDouble();
}

bool operator==(const WorkunitInfo &lhs, const WorkunitInfo &rhs)
{
	return lhs.m_attributes == rhs.m_attributes;
}

bool operator!=(const WorkunitInfo &lhs, const WorkunitInfo &rhs)
{
	return lhs.m_attributes != rhs.m_attributes;
}

} /* end of namespace InfoBoinc */

