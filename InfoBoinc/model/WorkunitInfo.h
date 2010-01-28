/*
 * =====================================================================
 *
 *       Filename:  WorkunitInfo.h
 *
 *        Version:  1.0
 *        Created:  22.01.2010 05:58:13
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef WORKUNITINFO_H
#define WORKUNITINFO_H

#include "XMLAttributeMap.h"
class QDomElement;

namespace InfoBoinc {

/**
 * \class WorkunitInfo
 */
class WorkunitInfo
{
public:
	/* ====================  LIFECYCLE     ==================== */
	WorkunitInfo();
	WorkunitInfo(const QDomElement &workunitInfoNode);
	~WorkunitInfo();

	/* ====================  ACCESSORS     ==================== */
	bool isNull() const;
	QString primaryKey() const;
	QString name() const;
	QString appnName() const;
	QString versionNum() const;
	double rscFpopsEst() const;
	double rscFpopsBound() const;
	double rscMemoryBound() const;
	double rscDiskBound() const;

	/* ====================  OPERATORS     ==================== */
	friend bool operator==(const WorkunitInfo &lhs, const WorkunitInfo &rhs);
	friend bool operator!=(const WorkunitInfo &lhs, const WorkunitInfo &rhs);

private:
	static QMap<QString, int> m_workunitAttributeTypes;
	XMLAttributeMap m_attributes;
}; /* -----  end of class WorkunitInfo  ----- */

bool operator==(const WorkunitInfo &lhs, const WorkunitInfo &rhs);
bool operator!=(const WorkunitInfo &lhs, const WorkunitInfo &rhs);

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef WORKUNITINFO_H  ----- */

