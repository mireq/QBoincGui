/*
 * =====================================================================================
 *
 *       Filename:  Socket.cpp
 *
 *        Version:  1.0
 *        Created:  31.12.2009 14:56:09
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#include <QtCore/QMetaType>

#include "config.h"
#include "debug.h"
#include "Socket.h"

Q_DECLARE_METATYPE(QAbstractSocket::SocketState)
Q_DECLARE_METATYPE(QAbstractSocket::SocketError)

namespace InfoBoinc {

Socket::Socket(QObject *parent)
	: QThread(parent),
	  m_host(),
	  m_port(0),
	  m_socket(0),
	  m_state(Unconnected),
	  m_msgFrontLength(0)
{
	qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
	qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}


Socket::~Socket()
{
	disconnectFromBoinc();
}


void Socket::connectToBoinc(const QString &host, quint16 port)
{
	disconnectFromBoinc();
	m_host = host;
	m_port = port;
	start();
}


void Socket::disconnectFromBoinc()
{
	m_sockMutex.lock();
	if (m_socket != 0) {
		m_socket->disconnectFromHost();
		m_socket->deleteLater();
		m_socket = 0;
	}
	// Vyprázdnime buffer správ
	m_msgFrontMutex.lock();
	while (!m_msgFront.empty()) {
		m_msgFrontLength.acquire(m_msgFront.length());
		for (int i = 0; i < m_msgFront.length(); ++i) {
			emit dataRecived(QByteArray());
		}
		m_msgFront.clear();
	}
	m_msgFrontMutex.unlock();
	m_sockMutex.unlock();
}


Socket::State Socket::state()
{
	return m_state;
}


void Socket::sendData(const QByteArray &data)
{
	m_msgFrontMutex.lock();
	m_msgFront.append(data);
	m_msgFrontMutex.unlock();
	m_msgFrontLength.release();
}


void Socket::updateSocketState(QAbstractSocket::SocketState socketState)
{
	State newState = Unconnected;
	switch(socketState)
	{
		case QAbstractSocket::UnconnectedState: newState = Unconnected; break;
		// Oba stavy označuj pripájanie sa
		case QAbstractSocket::HostLookupState:
		case QAbstractSocket::ConnectingState:  newState = Connecting; break;
		case QAbstractSocket::ConnectedState:   newState = Connected; break;
		case QAbstractSocket::ClosingState:     newState = Disconnecting; break;
		default: break;
	}
	if (newState != m_state) {
		if (newState == Unconnected) {
			disconnectFromBoinc();
		}
		m_state = newState;
#ifdef DEBUG_BOINC_COMMUNICATION
		qDebug() << debugMsgInfo(this, "State") << m_state;
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
		emit stateChanged(m_state);
	}
}

void Socket::emitError()
{
	m_sockMutex.lock();
	if (m_socket != 0) {
		QString errorString = m_socket->errorString();
		if (!errorString.isEmpty()) {
#ifdef DEBUG_BOINC_COMMUNICATION
			qDebug() << debugMsgInfo(this, "Socket Error") << errorString;
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
			emit error(errorString);
		}
	}
	m_sockMutex.unlock();
}


void Socket::run()
{
	m_sockMutex.lock();
	if (m_socket != 0) {
		m_socket->deleteLater();
		m_socket = 0;
	}
	m_socket = new QTcpSocket;
	connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(updateSocketState(QAbstractSocket::SocketState)));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(emitError()));
	m_socket->connectToHost(m_host, m_port);

	m_sockMutex.unlock();
	if (m_socket->waitForConnected(30000)) {
#ifdef DEBUG_BOINC_COMMUNICATION
		qDebug() << debugMsgInfo(this) << "Connected to boinc client!";
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
	}
	else {
		disconnectFromBoinc();
		return;
	}
	forever {
		m_msgFrontLength.acquire();
		m_msgFrontMutex.lock();
		const QByteArray data = m_msgFront.takeFirst();
		m_msgFrontMutex.unlock();

		// Ukončenie vlákna
		if (data.isNull()) {
			return;
		}

		// Zápi dát
		m_sockMutex.lock();
		if (m_socket == 0) {
			m_sockMutex.unlock();
			emit dataRecived(QByteArray());
			return;
		}
		m_socket->write(data);
		m_socket->write("\003", 1);
		m_socket->flush();
		m_sockMutex.unlock();
#ifdef DEBUG_BOINC_COMMUNICATION
		qDebug() << debugMsgInfo(this, "Out") << data;
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */

		// Čítanie dát
		m_sockMutex.lock();
		if (m_socket == 0) {
			m_sockMutex.unlock();
			emit dataRecived(QByteArray());
			return;
		}
		m_socket->waitForReadyRead(10000);
		QByteArray ret = m_socket->readAll();
		m_sockMutex.unlock();
#ifdef DEBUG_BOINC_COMMUNICATION
		qDebug() << debugMsgInfo(this, "In") << ret;
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
		emit dataRecived(ret);
		disconnectFromBoinc();
	}
}


#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, Socket::State state)
{
	dbg.nospace() << "Socket::";
	switch(state) {
		case Socket::Unconnected:   dbg << "Unconnected"; break;
		case Socket::Connecting:    dbg << "Connecting"; break;
		case Socket::Connected:     dbg << "Connected"; break;
		case Socket::Disconnecting: dbg << "Disconnecting"; break;
	}
	return dbg.space();
}
#endif

} /* end of namespace InfoBoinc */
