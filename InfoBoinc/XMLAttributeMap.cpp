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
				case BoolAttribute:   value = QVariant(value.toBool());   break;
				case IntAttribute:    value = QVariant(value.toInt());    break;
				case DoubleAttribute: value = QVariant(value.toDouble()); break;
				default: break;
			}
			m_attributes[attrInfoIter.key()] = value;
		}
	}
}

} /* end of namespace InfoBoinc */

