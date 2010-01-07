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

Socket::Socket(QObject *parent):
	QThread(parent),
	m_host(),
	m_port(0),
	m_socket(0),
	m_state(UnconnectedState),
	m_msgFrontLength(0)
{
	connect(this, SIGNAL(finished()), SLOT(setFinishedState()));
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
	// Vyprázdnime buffer správ
	clearInputBuffer();
	m_msgFrontMutex.lock();
	if (isRunning()) {
		m_msgFront.append(QByteArray());
		m_msgFrontLength.release();
	}
	m_msgFrontMutex.unlock();
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
	State newState = UnconnectedState;
	switch(socketState)
	{
		case QAbstractSocket::UnconnectedState: newState = UnconnectedState; break;
		// Oba stavy označuj pripájanie sa
		case QAbstractSocket::HostLookupState:
		case QAbstractSocket::ConnectingState:  newState = ConnectingState; break;
		case QAbstractSocket::ConnectedState:   newState = ConnectedState; break;
		case QAbstractSocket::ClosingState:     newState = DisconnectingState; break;
		default: break;
	}

	if (newState == UnconnectedState) {
		disconnectFromBoinc();
	}
	else {
		setState(newState);
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


void Socket::setFinishedState()
{
	setState(UnconnectedState);
}


void Socket::clearInputBuffer()
{
	m_msgFrontMutex.lock();
	while (!m_msgFront.empty()) {
		m_msgFrontLength.acquire(m_msgFront.length());
		for (int i = 0; i < m_msgFront.length(); ++i) {
			if (!m_msgFront.at(i).isNull()) {
				emit dataRecived(QByteArray());
			}
		}
		m_msgFront.clear();
	}
	m_msgFrontMutex.unlock();
}


void Socket::run()
{
	m_sockMutex.lock();
	if (m_socket != 0) {
		delete m_socket;
		m_socket = 0;
	}
	m_socket = new QTcpSocket;
	connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(updateSocketState(QAbstractSocket::SocketState)));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(emitError()));
	m_socket->connectToHost(m_host, m_port);

	m_sockMutex.unlock();
	if (m_socket->waitForConnected(ConnectWait)) {
#ifdef DEBUG_BOINC_COMMUNICATION
		qDebug() << debugMsgInfo(this) << "Connected to boinc client!";
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
	}
	else {
		disconnectFromBoinc();
	}
	forever {
		m_msgFrontLength.acquire();
		m_msgFrontMutex.lock();
		const QByteArray data = m_msgFront.takeFirst();
		m_msgFrontMutex.unlock();

		// Ukončenie vlákna
		if (data.isNull()) {
			break;
		}

		// Zápis dát
		m_sockMutex.lock();
		if (m_socket == 0) {
			m_sockMutex.unlock();
			emit dataRecived(QByteArray());
			break;
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
		QByteArray ret;
		if (m_socket == 0) {
			m_sockMutex.unlock();
			emit dataRecived(QByteArray());
			break;
		}
		forever {
			m_socket->waitForReadyRead(ReadWait);
			const QByteArray dataPart = m_socket->readAll();
			ret.append(dataPart);
			if (dataPart.isNull() || dataPart.size() == 0) {
				break;
			}
			else if (dataPart[dataPart.size() - 1] == '\003') {
				ret.resize(ret.size() - 1);
				break;
			}
		}
		m_sockMutex.unlock();
#ifdef DEBUG_BOINC_COMMUNICATION
		qDebug() << debugMsgInfo(this, "In") << ret;
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
		emit dataRecived(ret);
	}

	m_sockMutex.lock();
	if (m_socket != 0) {
		m_socket->disconnectFromHost();
		if (m_socket->state() == QAbstractSocket::ClosingState) {
			m_socket->waitForDisconnected(DisconnectWait);
		}
		delete m_socket;
		m_socket = 0;
	}
	m_sockMutex.unlock();
	clearInputBuffer();
}


void Socket::setState(State state)
{
	if (state == m_state) {
		return;
	}
	m_state = state;
#ifdef DEBUG_BOINC_COMMUNICATION
	qDebug() << debugMsgInfo(this, "State") << m_state;
#endif    /* ----- #ifndef DEBUG_BOINC_COMMUNICATION ----- */
	emit stateChanged(m_state);
}


#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, Socket::State state)
{
	dbg.nospace() << "Socket::";
	switch(state) {
		case Socket::UnconnectedState:   dbg << "UnconnectedState"; break;
		case Socket::ConnectingState:    dbg << "ConnectingState"; break;
		case Socket::ConnectedState:     dbg << "ConnectedState"; break;
		case Socket::DisconnectingState: dbg << "DisconnectingState"; break;
	}
	return dbg.space();
}
#endif

} /* end of namespace InfoBoinc */
