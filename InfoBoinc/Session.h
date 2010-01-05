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
#include "Socket.h"

namespace InfoBoinc {

/**
 * \class Session
 * \brief Komunikačné rozhranie s BOINC-om.
 */
class Session: public QObject
{
Q_OBJECT
public:
	enum ErrorType {
		UnauthorizedError
	};

	enum State {
		UnconnectedState,
		ConnectingState,
		ConnectedState,
		DisconnectingState
	};

	Session(QObject *parent = 0);
	~Session();
	quint32 id() const;
	Session::State state() const;

public slots:
	void openSession(const QString &host, quint16 port, const QString &password);
	void closeSession();
	void requestProjectStatus();

signals:
	void error(Session::ErrorType code, const QString &message = QString());
	void stateChanged(Session::State state);

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
}; /* -----  end of class Session  ----- */

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, Session::State state);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef SESSION_H  ----- */

