/*
 * =====================================================================
 *
 *       Filename:  ResultInfo.cpp
 *
 *        Version:  1.0
 *        Created:  22.01.2010 07:01:48
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "ResultInfo.h"

namespace InfoBoinc {

QMap<QString, int> ResultInfo::m_resultAttributeTypes;
QMap<QString, int> ResultInfo::m_activeTaskAttributeTypes;

ResultInfo::ResultInfo()
{
}

ResultInfo::ResultInfo(const QDomElement &resultNode)
{
	if (m_resultAttributeTypes.count() == 0) {
		m_resultAttributeTypes["name"] = XMLAttributeMap::StringAttribute;
		m_resultAttributeTypes["wu_name"] = XMLAttributeMap::StringAttribute;
		m_resultAttributeTypes["state"] = XMLAttributeMap::IntAttribute;
	}
	if (m_activeTaskAttributeTypes.count() == 0) {
		m_activeTaskAttributeTypes["fraction_done"] = XMLAttributeMap::DoubleAttribute;
		m_activeTaskAttributeTypes["active_task_state"] = XMLAttributeMap::DoubleAttribute;
	}
	m_attributes.parseAttributes(resultNode, m_resultAttributeTypes);
	QDomNodeList activeTaskNodes = resultNode.elementsByTagName("active_task");
	if (activeTaskNodes.count() > 0) {
		m_attributes.parseAttributes(activeTaskNodes.at(0).toElement(), m_activeTaskAttributeTypes);
	}
	qDebug() << m_attributes;
}

ResultInfo::~ResultInfo()
{
}

void ResultInfo::bindWorkunit(const WorkunitInfo &workunit)
{
	m_workunit = workunit;
}

bool ResultInfo::isNull()
{
	return m_attributes.isNull();
}

const WorkunitInfo &ResultInfo::workunit() const
{
	return m_workunit;
}

QString ResultInfo::primaryKey() const
{
	return m_attributes.attribute("name").toString();
}

QString ResultInfo::name() const
{
	return m_attributes.attribute("name").toString();
}

QString ResultInfo::wuName() const
{
	return m_attributes.attribute("wu_name").toString();
}

int ResultInfo::state() const
{
	return m_attributes.attribute("state").toInt();
}

double ResultInfo::fractionDone() const
{
	return m_attributes.attribute("fraction_done").toDouble();
}

int ResultInfo::activeTaskState() const
{
	return m_attributes.attribute("active_task_state").toInt();
}

bool operator==(const ResultInfo &lhs, const ResultInfo &rhs)
{
	return (lhs.m_attributes == rhs.m_attributes) && (lhs.m_workunit == rhs.m_workunit);
}

bool operator!=(const ResultInfo &lhs, const ResultInfo &rhs)
{
	return (lhs.m_attributes != rhs.m_attributes) || (lhs.m_workunit != rhs.m_workunit);
}

} /* end of namespace InfoBoinc */

