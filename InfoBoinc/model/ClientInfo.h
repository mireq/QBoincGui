/*
 * =====================================================================
 *
 *       Filename:  ClientInfo.h
 *
 *        Version:  1.0
 *        Created:  19.01.2010 16:43:01
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QtCore/QString>
#include "XMLAttributeMap.h"
class QDomElement;

namespace InfoBoinc {

/**
 * \class ClientInfo
 */
class ClientInfo
{
public:
	/* ====================  LIFECYCLE     ==================== */
	ClientInfo();
	ClientInfo(const QDomElement &clientInfoNode);
	~ClientInfo();

	/* ====================  ACCESSORS     ==================== */
	bool isNull() const;
	QString platformName() const;
	int coreClientMajorVersion() const;
	int coreClientMinorVersion() const;
	int coreClientRelease() const;
	QString coreClientVersion() const;
	QStringList platform() const;

	/* ====================  OPERATORS     ==================== */
	friend bool operator==(const ClientInfo &lhs, const ClientInfo &rhs);
	friend bool operator!=(const ClientInfo &lhs, const ClientInfo &rhs);

private:
	static QMap<QString, int> m_attributeTypes;
	XMLAttributeMap m_attributes;
}; /* -----  end of class ClientInfo  ----- */

bool operator==(const ClientInfo &lhs, const ClientInfo &rhs);
bool operator!=(const ClientInfo &lhs, const ClientInfo &rhs);

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef CLIENTINFO_H  ----- */

