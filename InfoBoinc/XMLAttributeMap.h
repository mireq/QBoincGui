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
 * \brief Prevod atribútov z XML do mapy atribútov.
 */
class XMLAttributeMap
{
public:
	/**
	 * Typ položky v XML súbore.
	 */
	enum AttributeType {
		StringAttribute, /**< Textový atribút.            */
		BoolAttribute,   /**< Hodnoty \e true / \e false. */
		IntAttribute,    /**< Celé číslo.                 */
		DoubleAttribute  /**< Desatinné číslo.            */
	};

	XMLAttributeMap();
	~XMLAttributeMap();

	/**
	 * Vráti atribút s názvom \a name, alebo \e null, ak tento atribút
	 * neexistuje.
	 */
	QVariant attribute(const QString &name) const;

	/**
	 * Spracovanie časti XML (\a dom) do mapy atribútov. Typy a názvy atribútov
	 * určuje map \a attributes. Funkcia predpokladá xml súbor vo takomto
	 * formáte:
	 * \verbatim
 <dokument>
   <string_atribut>abcd</string_atribut>
   <binarny_atribut>true</binarny_atribut>
   <ciselny_atribut>123</ciselny_atribut>
   <desatinny_atribut>123.456</desatinny_atribut>
 </dokument> \endverbatim
	 */
	void parseAttributes(const QDomElement &dom, const QMap<QString,AttributeType> &attributes);

private:
	QMap<QString, QVariant> m_attributes;
}; /* -----  end of class XMLAttributeMap  ----- */

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef XMLATTRIBUTEMAP_H  ----- */

