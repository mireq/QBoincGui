/*
 * =====================================================================================
 *
 *       Filename:  Session.h
 *
 *        Version:  1.0
 *        Created:  04.01.2010 10:33:34
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef SESSION_H
#define SESSION_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtXml/QDomNode>
#include "model/ProjectInfo.h"
#include "Socket.h"

namespace InfoBoinc {

/**
 * \class Session
 * \brief Komunikačné rozhranie s BOINC-om.
 */
class Session: public QObject
{
Q_OBJECT

/**
 * Stav, v ktorom sa nachádza komunikačný kanál s BOINC-om.
 */
Q_PROPERTY(State state READ state NOTIFY stateChanged)
public:
	/**
	 * Typ reprezentujúci ID spojenia s BOINC-om.
	 */
	typedef quint32 IdType;

	/**
	 * Typ chyby, ktorá nastala.
	 */
	enum ErrorType {
		UnauthorizedError /**< Chyba pri autorizácii. */
	};

	/**
	 * Stav spojenia ku klientu BOINC.
	 */
	enum State {
		UnconnectedState,  /**< Nepripojený.                     */
		ConnectingState,   /**< Pripájanie a autorizácia.        */
		ConnectedState,    /**< Pripojený a autorizovanýs.       */
		DisconnectingState /**< Uzatváranie spojenia s klientom. */
	};

	/**
	 * Vytvorenie nového komunikačného rozhrania.
	 */
	Session(QObject *parent = 0);

	/**
	 * Zrušenie komunikačného rozhrania.
	 */
	~Session();

	/**
	 * Vráti unikátne číslo spojenia.
	 */

	IdType id() const;

	/**
	 * Vráti aktuálny stav spojenia s BOINC-om.
	 * \sa stateChanged()
	 */
	Session::State state() const;

	/**
	 * Vráti host, s ktorým spája.
	 */
	const QString &host() const;

	/**
	 * Vráti port, ktorým sa session pripája na klienta.
	 */
	quint16 port() const;

	/**
	 * Vráti heslo, ktoré sa používa pre pripojenie ku klientovi.
	 */
	const QString &password() const;

	/**
	 * Nastavenie pracovného adresára na \a directory;
	 */
	void setDirectory(const QString &directory);

public slots:
	/**
	 * Otvorenie spojenia s klientom bežiacom na stroji \a host počúvajúcom na
	 * porte \a port s heslom \a password. Heslo sa štandardne nachádza v
	 * súbore gui_rpc_auth.cfg v pracovnom adresári BOINC klienta.
	 * \warning Táto funkcia je asynchrónna, úspešnosť spojenia indikuje signál
	 * stateChanged().
	 */
	void openSession(const QString &host, quint16 port, const QString &password);

	/**
	 * Uzatvorenie spojenia s BOINC-om.
	 * \warning Táto funkcia je asynchrónna, odpojenie indikuje signál
	 * stateChanged().
	 */
	void closeSession();

	/**
	 * Vyžiadanie stavu projektov.
	 */
	void requestProjectStatus();

	const ProjectInfo &project(const QString &projectId) const;

signals:
	/**
	 * Signál sa emituje v dôsledku chyby spôsobenej pri komunikácii s klientom,
	 * alebo chyby socketu.
	 */
	void error(Session::ErrorType code, const QString &message = QString());

	/**
	 * Signál sa emituje pri zmene stavu spojenia.
	 * \sa state()
	 */
	void stateChanged(Session::State state, Session::IdType id);

	void projectAdded(const QString &projectId, Session::IdType id);
	void projectRemoved(const QString &projectId, Session::IdType id);
	void projectChanged(const QString &projectId, Session::IdType id);

private slots:
	void processData(const QByteArray &data);
	void socketStateChanged(Socket::State state);

private:
	Q_DISABLE_COPY(Session)

	static const unsigned long DisconnectWait = 2000;

	typedef void (Session::*TProcessDataCallback)(const QByteArray &data);

	void setState(State state);
	void triggerXmlError(const QDomNode &reply = QDomNode());
	QDomElement getReply(const QByteArray &data);
	void sendCommand(const QByteArray &command, TProcessDataCallback callback);
	void startAuthorisation();

	void createProjectData(const QString &projectId);
	void removeProjectData(const QString &projectId);

	/* ====================  CALLBACKS     ==================== */
	void processAuth1(const QByteArray &data);
	void processAuth2(const QByteArray &data);
	void processProjectStatus(const QByteArray &data);

	/* ====================  STATIC        ==================== */
	static quint32 m_nextId;

	/* ====================  DATA MEMBERS  ==================== */
	Socket *m_socket;
	State m_state;
	quint32 m_id;
	QList<TProcessDataCallback> m_processDataCallbacks;

	QString m_host;
	quint16 m_port;
	QString m_password;
	QString m_directory;

	/* ====================  MODEL DATA    ==================== */
	QMap<QString, ProjectInfo> m_projects;
}; /* -----  end of class Session  ----- */

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, Session::State state);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef SESSION_H  ----- */

