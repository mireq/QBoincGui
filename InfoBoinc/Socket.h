/*
 * =====================================================================================
 *
 *       Filename:  Socket.h
 *
 *        Version:  1.0
 *        Created:  31.12.2009 14:56:34
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>

namespace InfoBoinc {

/**
 * \class Socket
 */
class Socket: public QThread
{
Q_OBJECT
Q_ENUMS(State)
Q_PROPERTY(State state READ state NOTIFY stateChanged)
public:
	Socket(QObject *parent = 0);
	~Socket();
	void connectToBoinc(const QString &host, quint16 port);
	void disconnectFromBoinc();

	enum State {
		Unconnected,
		Connecting,
		Connected,
		Disconnecting
	};
	State state();

	void sendData(const QByteArray &data);

signals:
	void stateChanged(Socket::State state);
	void dataRecived(const QByteArray &data);
	void error(const QString &error);

private slots:
	void updateSocketState(QAbstractSocket::SocketState socketState);
	void emitError();

private:
	virtual void run();

	/* ====================  DATA MEMBERS  ==================== */
	QString m_host;
	quint16 m_port;
	QTcpSocket *m_socket;
	State m_state;
	QSemaphore m_msgFrontLength;
	QMutex m_msgFrontMutex;
	QMutex m_sockMutex;
	QList<QByteArray> m_msgFront;
}; /* -----  end of class Socket  ----- */

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, Socket::State state);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef SOCKET_H  ----- */

