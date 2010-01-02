/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *        Version:  1.0
 *        Created:  01.01.2010 15:11:10
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

/**
 * \file
 * \brief Pomocné funkcie pre výpis ladiacich správ.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <QtCore/QDebug>

/**
 * \brief Trieda obsahujúca metainformácie o ladiacej správe.
 */
class MsgInfo {
public:
	/**
	 * Vytvorenie neovej informácie o správe generovanej modulom \a module typu
	 * \a msgType.
	 */
	MsgInfo(const QString &module, const QString &msgType = QString()):
		m_module(module),
		m_msgType(msgType)
	{};

	/**
	 * \overload
	 */
	MsgInfo(QObject *module, const QString &msgType = QString()):
		m_module(module->metaObject()->className()),
		m_msgType(msgType)
	{};

	/**
	 * Nastavenie zdrojového modulu na hodnotu \a module.
	 */
	void setModule(const QString &module)
	{ m_module = module; };

	/**
	 * \overload
	 */
	void setModule(QObject *module)
	{ m_module = module->metaObject()->className(); };

	/**
	 * Nastavenie typu správy na \a msgType.
	 */
	void setMsgType(const QString &msgType)
	{ m_msgType = msgType; };

	/**
	 * Funkcia vráti názov modulu.
	 */
	const QString &module() const
	{ return m_module; };

	/**
	 * Funkcia vráti typ správy.
	 */
	const QString &msgType() const
	{ return m_msgType; };
private:
	QString m_module;
	QString m_msgType;
};

/**
 * \brief Výpis objektu \ref MsgInfo.
 *
 * Tento operátor je zodpovedný za výpis metadát o type správy (\ref MsgInfo).
 */
QDebug operator<<(QDebug, const MsgInfo &mi);

/**
 * \brief Vylepšený výpis ladiacich správ.
 *
 * Táto funkcia umožňuje prehľadný výpis ladiacich správ na konzolu.
 * Formátovanie správ umožňuje do správy vložiť zdroj správy (názov modulu,
 * ktorý správu generoval) a voliteľne typ správy. Formát správy je: <tt>\<Názov
 * modulu|Typ správy\> Text správy</tt>, alebo bez určeného typu <tt>\<Názov
 * modulu\> Text správy</tt>. Úvodná časť správy sa dá vygenerovať funkciou
 * \ref debugMsgInfo.
 *
 * Špeciálnym typom správ sú "In" a "Out" označujúce čítanie / zápis dát. Dáta
 * sú v tomto prípade zobrazené na nasledujúcom riadku, nezačínajú teda v
 * rovnakom riadku ako správa.
 *
 * Funkcia sa musí registrovať volaním qInstallMsgHandler.
 */
void infoBoincMsgHandler(QtMsgType type, const char *msg);

/**
 * \brief Vygenerovanie informácie o správe.
 *
 * Funkcia vygeneruje informáciu o správe s názvom modulu \a module a voliteľným
 * typom správy \a msgType.
 * \code
 * qDebug() << debugMsgInfo("Modul", "In") << data;
 * \endcode
 */
MsgInfo debugMsgInfo(const QString &module, const QString &msgType = QString());

/**
 * \overload
 * Funkcia vygeneruje názov modulu na základe názvu triedy získanej z objektu \a
 * object. Voliteľne môže mať typ správy \a msgType.
 * \code
 * qDebug() << debugMsgInfo(this, "In") << data;
 * \endcode
 */
MsgInfo debugMsgInfo(QObject *object, const QString &msgType = QString());

#endif    /* ----- #ifndef DEBUG_H  ----- */

