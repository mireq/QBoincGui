/*
 * =====================================================================================
 *
 *       Filename:  ProjectInfo.h
 *
 *        Version:  1.0
 *        Created:  05.01.2010 16:47:33
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include "XMLAttributeMap.h"
class QDomElement;

namespace InfoBoinc {
/**
 * \class ProjectInfo
 */
class ProjectInfo
{
public:
	/* ====================  LIFECYCLE     ==================== */
	ProjectInfo(const QDomElement &projectInfoNode);
	~ProjectInfo();

	/* ====================  ACCESSORS     ==================== */
	QString masterUrl() const;
	QString projectName() const;

	/* ====================  MUTATORS      ==================== */

private:
	static QMap<QString, XMLAttributeMap::AttributeType> m_attributeTypes;
	XMLAttributeMap m_attributes;
	/* ====================  DATA MEMBERS  ==================== */
}; /* -----  end of class ProjectInfo  ----- */
} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef PROJECTINFO_H  ----- */

