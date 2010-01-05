/*
 * =====================================================================================
 *
 *       Filename:  Session.cpp
 *
 *        Version:  1.0
 *        Created:  04.01.2010 10:33:31
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#include <QtCore/QCryptographicHash>
#include "debug.h"
#include "model/ProjectInfo.h"
#include "Session.h"

namespace InfoBoinc {

quint32 Session::m_nextId = 1;

Session::Session(QObject *parent)
	: QObject(parent),
	  m_socket(0),
	  m_state(UnconnectedState),
	  m_id(m_nextId)
{
	m_nextId++;
}


Session::~Session()
{
	if (m_socket != 0) {
		m_socket->deleteLater();
		m_socket = 0;
	}
}


quint32 Session::id() const
{
	return m_id;
}


Session::State Session::state() const
{
	return m_state;
}


void Session::openSession(const QString &host, quint16 port, const QString &password)
{
	closeSession();
	setState(ConnectingState);
	m_host = host;
	m_port = port;
	m_password = password;
	m_socket = new Socket;
	connect(m_socket, SIGNAL(stateChanged(Socket::State)), SLOT(socketStateChanged(Socket::State)));
	connect(m_socket, SIGNAL(dataRecived(const QByteArray &)), SLOT(processData(const QByteArray &)));
	m_socket->connectToBoinc(m_host, m_port);
}


void Session::closeSession()
{
	if (m_state == UnconnectedState) {
		return;
	}
	setState(DisconnectingState);
	if (m_socket != 0) {
		m_socket->disconnectFromBoinc();
	}
}


void Session::requestProjectStatus()
{
	sendCommand("<boinc_gui_rpc_request><get_project_status /></boinc_gui_rpc_request>", &Session::processProjectStatus);
}


void Session::processData(const QByteArray &data)
{
	TProcessDataCallback callback = m_processDataCallbacks.takeFirst();
	if (callback != 0) {
		(this->*callback)(data);
	}
}


void Session::socketStateChanged(Socket::State state)
{
	if (state == Socket::ConnectedState) {
		startAuthorisation();
	}
	else if (m_socket != 0 && state == Socket::UnconnectedState) {
		m_socket->deleteLater();
		m_socket = 0;
		setState(UnconnectedState);
	}
}


void Session::setState(State state)
{
	if (state != m_state) {
	}
	m_state = state;
	emit stateChanged(state);
}


void Session::triggerXmlError(const QDomNode &reply)
{
	if (reply.nodeName() == "error") {
		qDebug() << reply.firstChild().nodeValue();
	}
}


QDomElement Session::getReply(const QByteArray &data)
{
	QDomDocument doc;
	QString error;
	if (!doc.setContent(data, false, &error)) {
		qWarning() << debugMsgInfo(this) << "XML parsing error" << error;
		return QDomElement();
	}

	QDomElement rpcReply = doc.documentElement();
	if (rpcReply.isNull() || rpcReply.nodeName() != "boinc_gui_rpc_reply") {
		qWarning() << debugMsgInfo(this) << "Bad XML root element" << rpcReply.nodeName();
		return QDomElement();
	}

	QDomNodeList nodes = rpcReply.childNodes();
	for (int i = 0; i < nodes.count(); ++i) {
		QDomNode node = nodes.item(i);
		if (node.isElement()) {
			return node.toElement();
		}
	}
	return QDomElement();
}


void Session::sendCommand(const QByteArray &command, TProcessDataCallback callback)
{
	m_processDataCallbacks.append(callback);
	m_socket->sendData(command);
}


void Session::startAuthorisation()
{
	sendCommand("<boinc_gui_rpc_request><auth1 /></boinc_gui_rpc_request>", &Session::processAuth1);
}


void Session::processAuth1(const QByteArray &data)
{
	QDomElement reply = getReply(data);
	if (reply.nodeName() != "nonce") {
		triggerXmlError(reply);
		return;
	}

	QString nonce = reply.firstChild().nodeValue();
	QCryptographicHash hashGenerator(QCryptographicHash::Md5);
	QString hashData = nonce + m_password;
	QByteArray hash = hashGenerator.hash(hashData.toUtf8(), QCryptographicHash::Md5);
	sendCommand(QString("<boinc_gui_rpc_request><auth2><nonce_hash>%1</nonce_hash></auth2></boinc_gui_rpc_request>").arg(QString(hash.toHex())).toUtf8(), &Session::processAuth2);
}


void Session::processAuth2(const QByteArray &data)
{
	QDomNode reply = getReply(data);
	if (reply.nodeName() != "authorized" && reply.nodeName() != "unauthorized") {
		triggerXmlError(reply);
		return;
	}

	if (reply.nodeName() == "authorized") {
		qDebug() << debugMsgInfo(this) << "Authorized";
		setState(ConnectedState);
	}
	else {
		qDebug() << debugMsgInfo(this) << "Authorisation Failed";
		closeSession();
		emit error(UnauthorizedError);
	}
}


void Session::processProjectStatus(const QByteArray &data)
{
	QDomNode reply = getReply(data);
	if (reply.nodeName() != "projects") {
		triggerXmlError(reply);
		return;
	}

	for (int i = 0; i < reply.childNodes().count(); ++i) {
		QDomNode projectNode = reply.childNodes().at(i);
		if (!projectNode.isElement()) {
			continue;
		}
	}
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, Session::State state)
{
	dbg.nospace() << "Session::";
	switch(state) {
		case Session::UnconnectedState:   dbg << "UnconnectedState"; break;
		case Session::ConnectingState:    dbg << "ConnectingState"; break;
		case Session::ConnectedState:     dbg << "ConnectedState"; break;
		case Session::DisconnectingState: dbg << "DisconnectingState"; break;
	}
	return dbg.space();
}
#endif

} /* end of namespace InfoBoinc */

