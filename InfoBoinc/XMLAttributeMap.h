/*
 * =====================================================================================
 *
 *       Filename:  XMLAttributeMap.h
 *
 *        Version:  1.0
 *        Created:  05.01.2010 19:42:30
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef XMLATTRIBUTEMAP_H
#define XMLATTRIBUTEMAP_H

#include <QtCore/QMap>
#include <QtCore/QVariant>

class QDomElement;

namespace InfoBoinc {

/**
 * \class XMLAttributeMap
 */
class XMLAttributeMap
{
public:
	enum AttributeType {
		StringAttribute,
		BoolAttribute,
		IntAttribute,
		DoubleAttribute
	};

	XMLAttributeMap();
	~XMLAttributeMap();

	QVariant attribute(const QString &name) const;
	void parseAttributes(const QDomElement &dom, const QMap<QString,AttributeType> &attributes);

private:
	QMap<QString, QVariant> m_attributes;
}; /* -----  end of class XMLAttributeMap  ----- */

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef XMLATTRIBUTEMAP_H  ----- */

