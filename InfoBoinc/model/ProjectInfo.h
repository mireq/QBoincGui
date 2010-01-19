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

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include "debug.h"
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
	QString primaryKey() const;
	QString masterUrl() const;
	QString projectName() const;
	QString userName() const;
	QString teamName() const;
	QString hostVenue() const;
	double userTotalCredit() const;
	double userExpavgCredit() const;
	QDateTime userCreateTime() const;
	double hostTotalCredit() const;
	double hostExpavgCredit() const;
	QDateTime hostCreateTime() const;

	/* ====================  OPERATORS     ==================== */
	friend bool operator==(const ProjectInfo &lhs, const ProjectInfo &rhs);
	friend bool operator!=(const ProjectInfo &lhs, const ProjectInfo &rhs);

private:
	static QMap<QString, int> m_attributeTypes;
	XMLAttributeMap m_attributes;
	/* ====================  DATA MEMBERS  ==================== */
}; /* -----  end of class ProjectInfo  ----- */

bool operator==(const ProjectInfo &lhs, const ProjectInfo &rhs);
bool operator!=(const ProjectInfo &lhs, const ProjectInfo &rhs);

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const ProjectInfo &info);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef PROJECTINFO_H  ----- */

