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
#include "model/ClientInfo.h"
#include "model/FileTransferInfo.h"
#include "model/HostInfo.h"
#include "model/ProjectInfo.h"
#include "model/ResultInfo.h"
#include "model/WorkunitInfo.h"
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
	InfoBoinc::Session::State state() const;

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
	 * Vráti adresár ak je klient lokálny.
	 */
	const QString &directory() const;

	/**
	 * Vráti \e true, ak je to spojenie na lokálneho klienta.
	 */
	bool isLocal() const;

	QList<QString> projects() const;

	const ProjectInfo &project(const QString &projectId) const;

	const HostInfo &hostInfo() const;

	const ClientInfo &clientInfo() const;

	QList<FileTransferInfo> transfers() const;

	ResultInfo result(const QString &masterURL, const QString &name) const;

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

	/**
	 * Vyžiadanie informácií o hostiteľovi.
	 */
	void requestHostInfo();

	/**
	 * Požiadavka na zistenie komplexného stavu.
	 */
	void requestState();
	void requestClientInfo();

	void requestFileTransfers();

signals:
	/**
	 * Signál sa emituje v dôsledku chyby spôsobenej pri komunikácii s klientom,
	 * alebo chyby socketu.
	 */
	void error(InfoBoinc::Session::ErrorType code, const QString &message = QString());

	/**
	 * Signál sa emituje pri zmene stavu spojenia.
	 * \sa state()
	 */
	void stateChanged(InfoBoinc::Session::State state, InfoBoinc::Session::IdType id);

	void projectsAdded(const QList<QString> &projects, InfoBoinc::Session::IdType id);
	void projectsRemoved(const QList<QString> &projects, InfoBoinc::Session::IdType id);
	void projectsChanged(const QList<QString> &projects, InfoBoinc::Session::IdType id);

	void resultsAdded(const QMap<QString, QList<QString> > &results, InfoBoinc::Session::IdType id);
	void resultsRemoved(const QMap<QString, QList<QString> > &results, InfoBoinc::Session::IdType id);
	void resultsChanged(const QMap<QString, QList<QString> > &results, InfoBoinc::Session::IdType id);

	void hostInfoChanged(const InfoBoinc::HostInfo &hostInfo, InfoBoinc::Session::IdType id);
	void clientInfoChanged(const InfoBoinc::ClientInfo &clientInfo, InfoBoinc::Session::IdType id);

	void transferAdded(const InfoBoinc::FileTransferInfo &info, InfoBoinc::Session::IdType id);
	void transferRemoved(const InfoBoinc::FileTransferInfo &info, InfoBoinc::Session::IdType id);
	void transferChanged(const InfoBoinc::FileTransferInfo &info, InfoBoinc::Session::IdType id);

private slots:
	void processData(const QByteArray &data);
	void socketStateChanged(Socket::State state);

private:
	Q_DISABLE_COPY(Session)

	static const unsigned long DisconnectWait = 2000;

	typedef void (InfoBoinc::Session::*TProcessDataCallback)(const QByteArray &data);

	void setState(State state);
	void triggerXmlError(const QDomNode &reply = QDomNode());
	QDomElement getReply(const QByteArray &data);
	void sendCommand(const QByteArray &command, TProcessDataCallback callback);
	void startAuthorisation();

	void processProjects(const QList<ProjectInfo> &projects);
	void processClientInfo(const QDomElement &element);
	void processWorkunits(const QMap<QString, QList<QDomElement> > &workunitElements, const QMap<QString, QList<QDomElement> > &resultElements);
	void createProjectData(const QString &projectId);
	void removeProjectData(const QString &projectId);

	/* ====================  CALLBACKS     ==================== */
	void processAuth1(const QByteArray &data);
	void processAuth2(const QByteArray &data);
	void processProjectStatus(const QByteArray &data);
	void processHostInfo(const QByteArray &data);
	void processState(const QByteArray &data);
	void processFileTransfers(const QByteArray &data);

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
	HostInfo m_hostInfo;
	ClientInfo m_clientInfo;
	QMap<QPair<QString, QString>, FileTransferInfo> m_transfers;
	QMap<QString, QMap<QString, ResultInfo> > m_results;
}; /* -----  end of class Session  ----- */

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, InfoBoinc::Session::State state);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef SESSION_H  ----- */

