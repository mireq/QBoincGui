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
		StringAttribute,    /**< Textový atribút.            */
		BoolAttribute,      /**< Hodnoty \e true / \e false. */
		ExistsAttribute,    /**< Platí \e true, ak existuje. */
		IntAttribute,       /**< Celé číslo.                 */
		UIntAttribute,      /**< Celé nezáporné číslo.       */
		DoubleAttribute,    /**< Desatinné číslo.            */
		TimestampAttribute, /**< Časová pečiatka.            */
		ListAttribute = 1024 /**< Nastavením tohto flagu sa všetky atribúty s 
		týmto názvom vkladajú do zoznamu. S ostatnými typmi atribútov
		je kombinovateľný operátorom or (|) */
	};

	XMLAttributeMap();
	~XMLAttributeMap();

	/**
	 * Vráti \e true, ak nebol nastavený obsah pomocou
	 * XMLAttributeMap::parseAttributes.
	 */
	bool isNull() const;

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
	void parseAttributes(const QDomElement &dom, const QMap<QString,int> &attributes);

	/* ====================  OPERATORS     ==================== */
	/**
	 * Vráti \e true, ak sú 2 mapy atribútov identické.
	 */
	friend bool operator==(const XMLAttributeMap &lhs, const XMLAttributeMap &rhs);

	/**
	 * Vráti \e true, ak sú 2 mapy atribútov rozdielné.
	 */
	friend bool operator!=(const XMLAttributeMap &lhs, const XMLAttributeMap &rhs);

	/**
	 * Výpis atribútov spracovanej časti XML dát.
	 */
	friend QDebug operator<<(QDebug dbg, const XMLAttributeMap &atrMap);

private:
	QMap<QString, QVariant> m_attributes;
	bool m_isNull;
}; /* -----  end of class XMLAttributeMap  ----- */

bool operator==(const XMLAttributeMap &lhs, const XMLAttributeMap &rhs);
bool operator!=(const XMLAttributeMap &lhs, const XMLAttributeMap &rhs);

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const XMLAttributeMap &atrMap);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef XMLATTRIBUTEMAP_H  ----- */

