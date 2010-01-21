/*
 * =====================================================================================
 *
 *       Filename:  ProjectInfo.cpp
 *
 *        Version:  1.0
 *        Created:  05.01.2010 16:47:29
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */


#include "debug.h"
#include "ProjectInfo.h"

namespace InfoBoinc {

QMap<QString, int> ProjectInfo::m_attributeTypes;

ProjectInfo::ProjectInfo(const QDomElement &projectInfoNode)
{
	if (m_attributeTypes.count() == 0) {
		m_attributeTypes["master_url"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["project_name"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["user_name"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["team_name"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["host_venue"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["user_total_credit"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["user_expavg_credit"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["user_create_time"] = XMLAttributeMap::TimestampAttribute;
		m_attributeTypes["host_total_credit"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["host_expavg_credit"] = XMLAttributeMap::DoubleAttribute;
		m_attributeTypes["host_create_time"] = XMLAttributeMap::TimestampAttribute;
	}
	m_attributes.parseAttributes(projectInfoNode, m_attributeTypes);
}

ProjectInfo::~ProjectInfo()
{
}

QString ProjectInfo::primaryKey() const
{
	return m_attributes.attribute("master_url").toString();
}

QString ProjectInfo::masterURL() const
{
	return m_attributes.attribute("master_url").toString();
}

QString ProjectInfo::projectName() const
{
	return m_attributes.attribute("project_name").toString();
}

QString ProjectInfo::userName() const
{
	return m_attributes.attribute("user_name").toString();
}

QString ProjectInfo::teamName() const
{
	return m_attributes.attribute("team_name").toString();
}

QString ProjectInfo::hostVenue() const
{
	return m_attributes.attribute("host_venue").toString();
}

double ProjectInfo::userTotalCredit() const
{
	return m_attributes.attribute("user_total_credit").toDouble();
}

double ProjectInfo::userExpavgCredit() const
{
	return m_attributes.attribute("user_expavg_credit").toDouble();
}

QDateTime ProjectInfo::userCreateTime() const
{
	return m_attributes.attribute("user_create_time").toDateTime();
}

double ProjectInfo::hostTotalCredit() const
{
	return m_attributes.attribute("host_total_credit").toDouble();
}

double ProjectInfo::hostExpavgCredit() const
{
	return m_attributes.attribute("host_expavg_credit").toDouble();
}

QDateTime ProjectInfo::hostCreateTime() const
{
	return m_attributes.attribute("host_create_time").toDateTime();
}

bool operator==(const ProjectInfo &lhs, const ProjectInfo &rhs)
{
	return lhs.m_attributes == rhs.m_attributes;
}

bool operator!=(const ProjectInfo &lhs, const ProjectInfo &rhs)
{
	return lhs.m_attributes != rhs.m_attributes;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const ProjectInfo &info)
{
	dbg.nospace();
	dbg << "Master URL: " << info.masterURL() << "\n";
	dbg << "Project name: " << info.projectName() << "\n";
	dbg << "User name: " << info.userName() << "\n";
	if (!info.teamName().isNull()) {
		dbg << "Team name: " << info.teamName() << "\n";
	}
	if (!info.teamName().isNull()) {
		dbg << "Host venue: " << info.hostVenue() << "\n";
	}
	dbg << "User total credit: " << info.userTotalCredit() << "\n";
	dbg << "User expavg credit: " << info.userExpavgCredit() << "\n";
	dbg << "User create time: " << info.userCreateTime().toString(Qt::SystemLocaleShortDate) << "\n";
	dbg << "Host total credit: " << info.hostTotalCredit() << "\n";
	dbg << "Host expavg credit: " << info.hostExpavgCredit() << "\n";
	dbg << "Host create time: " << info.hostCreateTime().toString(Qt::SystemLocaleShortDate) << "\n";
	return dbg.space();
}
#endif

} /* end of namespace InfoBoinc */

