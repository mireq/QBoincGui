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

QMap<QString, XMLAttributeMap::AttributeType> ProjectInfo::m_attributeTypes;

ProjectInfo::ProjectInfo(const QDomElement &projectInfoNode)
{
	if (m_attributeTypes.count() == 0) {
		m_attributeTypes["master_url"] = XMLAttributeMap::StringAttribute;
		m_attributeTypes["project_name"] = XMLAttributeMap::StringAttribute;
	}
	m_attributes.parseAttributes(projectInfoNode, m_attributeTypes);
}

ProjectInfo::~ProjectInfo()
{
}

QString ProjectInfo::masterUrl() const
{
	return m_attributes.attribute("master_url").toString();
}

QString ProjectInfo::projectName() const
{
	return m_attributes.attribute("project_name").toString();
}

} /* end of namespace InfoBoinc */

