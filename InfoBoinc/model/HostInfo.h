/*
 * =====================================================================
 *
 *       Filename:  HostInfo.h
 *
 *        Version:  1.0
 *        Created:  18.01.2010 22:21:05
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QtCore/QString>
#include "XMLAttributeMap.h"
class QDomElement;

namespace InfoBoinc {

/**
 * \class HostInfo
 */
class HostInfo
{
public:
	/* ====================  LIFECYCLE     ==================== */
	HostInfo();
	HostInfo(const QDomElement &hostInfoNode);
	~HostInfo();

	/* ====================  ACCESSORS     ==================== */
	bool isNull() const;

	int timezone() const;
	QString domainName() const;
	QString ipAddr() const;
	int pCpus() const;
	QString pVendor() const;
	QString pModel() const;
	QString pFeatures() const;
	double pFpops() const;
	double pIops() const;
	double mNbytes() const;
	double mCache() const;
	double mSwap() const;
	double dTotal() const;
	double dFree() const;
	QString osName() const;
	QString osVersion() const;

	/* ====================  OPERATORS     ==================== */
	friend bool operator==(const HostInfo &lhs, const HostInfo &rhs);
	friend bool operator!=(const HostInfo &lhs, const HostInfo &rhs);

private:
	static QMap<QString, int> m_attributeTypes;
	XMLAttributeMap m_attributes;
}; /* -----  end of class HostInfo  ----- */

bool operator==(const HostInfo &lhs, const HostInfo &rhs);
bool operator!=(const HostInfo &lhs, const HostInfo &rhs);

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef HOSTINFO_H  ----- */

