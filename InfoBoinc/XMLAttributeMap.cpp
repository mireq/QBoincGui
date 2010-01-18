/*
 * =====================================================================================
 *
 *       Filename:  XMLAttributeMap.cpp
 *
 *        Version:  1.0
 *        Created:  05.01.2010 19:42:27
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#include <QtCore/QDateTime>
#include <QtXml/QDomNode>
#include "XMLAttributeMap.h"

namespace InfoBoinc {

XMLAttributeMap::XMLAttributeMap()
{
}


XMLAttributeMap::~XMLAttributeMap()
{
}


QVariant XMLAttributeMap::attribute(const QString &name) const
{
	QMap<QString,QVariant>::const_iterator attrIter = m_attributes.find(name);
	if (attrIter == m_attributes.end()) {
		return QVariant();
	}
	else {
		return attrIter.value();
	}
}


void XMLAttributeMap::parseAttributes(const QDomElement &dom, const QMap<QString,AttributeType> &attributes)
{
	QDomNodeList nodes = dom.childNodes();
	for (int i = 0; i < nodes.count(); ++i) {
		QDomNode node = nodes.at(i);
		if (!node.isElement()) {
			continue;
		}
		QMap<QString,AttributeType>::const_iterator attrInfoIter = attributes.find(node.nodeName());
		if (attrInfoIter != attributes.end()) {
			AttributeType type = attrInfoIter.value();
			QVariant value = node.toElement().text();
			switch (type) {
				case BoolAttribute:      value = QVariant(value.toBool());   break;
				case IntAttribute:       value = QVariant(value.toInt());    break;
				case UIntAttribute:      value = QVariant(value.toUInt());   break;
				case DoubleAttribute:    value = QVariant(value.toDouble()); break;
				case TimestampAttribute: value = QVariant(QDateTime::fromTime_t(uint(value.toDouble())));
				default: break;
			}
			m_attributes[attrInfoIter.key()] = value;
		}
	}
}

bool operator==(const XMLAttributeMap &lhs, const XMLAttributeMap &rhs)
{
	return lhs.m_attributes == rhs.m_attributes;
}

bool operator!=(const XMLAttributeMap &lhs, const XMLAttributeMap &rhs)
{
	return lhs.m_attributes != rhs.m_attributes;
}

QDebug operator<<(QDebug dbg, const XMLAttributeMap &atrMap)
{
	dbg.nospace();
	QMap<QString, QVariant>::const_iterator atrIterator;
	for (atrIterator = atrMap.m_attributes.begin(); atrIterator != atrMap.m_attributes.end(); ++atrIterator) {
		dbg << atrIterator.key() << ":" << atrIterator.value().toString() << "\n";
	}
	return dbg.space();
}

} /* end of namespace InfoBoinc */

