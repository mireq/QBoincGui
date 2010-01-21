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
#include <QtCore/QSet>
#include "debug.h"
#include "Session.h"

namespace InfoBoinc {

Session::IdType Session::m_nextId = 1;

Session::Session(QObject *parent):
	QObject(parent),
	m_socket(0),
	m_state(UnconnectedState),
	m_id(m_nextId)
{
	m_nextId++;
}


Session::~Session()
{
	if (m_socket != 0) {
		m_socket->disconnectFromBoinc();
		m_socket->wait(DisconnectWait);
		delete m_socket;
		m_socket = 0;
	}
}


Session::IdType Session::id() const
{
	return m_id;
}


Session::State Session::state() const
{
	return m_state;
}


const QString &Session::host() const
{
	return m_host;
}


quint16 Session::port() const
{
	return m_port;
}


const QString &Session::password() const
{
	return m_password;
}


const QString &Session::directory() const
{
	return m_directory;
}


bool Session::isLocal() const
{
	return !(m_directory.isNull());
}


const ProjectInfo &Session::project(const QString &projectId) const
{
	QMap<QString, ProjectInfo>::const_iterator info = m_projects.find(projectId);
	return info.value();
}


const HostInfo &Session::hostInfo() const
{
	return m_hostInfo;
}


const ClientInfo &Session::clientInfo() const
{
	return m_clientInfo;
}


QList<FileTransferInfo> Session::transfers() const
{
	return m_transfers.values();
}


void Session::setState(State state)
{
	if (state != m_state) {
		qDebug() << debugMsgInfo(this) << state;
		m_state = state;
		emit stateChanged(state, m_id);
	}
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


void Session::setDirectory(const QString &directory)
{
	m_directory = directory;
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


void Session::requestHostInfo()
{
	sendCommand("<boinc_gui_rpc_request><get_host_info /></boinc_gui_rpc_request>", &Session::processHostInfo);
}


void Session::requestState()
{
	sendCommand("<boinc_gui_rpc_request><get_state /></boinc_gui_rpc_request>", &Session::processState);
}


void Session::requestClientInfo()
{
	requestState();
}


void Session::requestFileTransfers()
{
	sendCommand("<boinc_gui_rpc_request><get_file_transfers /></boinc_gui_rpc_request>", &Session::processFileTransfers);
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
	if (m_socket != 0) {
		m_socket->sendData(command);
	}
	else {
		processData(QByteArray());
	}
}


void Session::startAuthorisation()
{
	sendCommand("<boinc_gui_rpc_request><auth1 /></boinc_gui_rpc_request>", &Session::processAuth1);
}


void Session::processProjectNodes(const QList<QDomElement> &projects)
{
	QSet<QString> projectsSet = QSet<QString>::fromList(m_projects.keys());
	QList<QString> added;
	QList<QString> removed;
	QList<QString> changed;

	// Zistenie zmien v projektoch a ich aktualizácia
	foreach(const QDomElement &projectElement, projects) {
		const ProjectInfo project(projectElement);
		const QString projectId = project.primaryKey();

		QSet<QString>::iterator projectIterator = projectsSet.find(project.primaryKey());

		// Projekt ešte nie je v zozname projektov
		if (projectIterator == projectsSet.end()) {
			m_projects.insert(projectId, project);
			createProjectData(projectId);
			added.append(projectId);
		}
		else {
			if (project != this->project(projectId)) {
				m_projects.insert(projectId, project);
				changed.append(projectId);
			}
			projectsSet.erase(projectIterator);
		}
	}

	foreach(const QString &projectId, projectsSet) {
		m_projects.remove(projectId);
		removeProjectData(projectId);
		removed.append(projectId);
	}

	if (!added.isEmpty()) {
		emit projectsAdded(added, m_id);
	}
	if (!removed.isEmpty()) {
		emit projectsRemoved(removed, m_id);
	}
	if (!changed.isEmpty()) {
		emit projectsChanged(changed, m_id);
	}
}


void Session::processClientInfo(const QDomElement &element)
{
	ClientInfo info(element);
	if (info != m_clientInfo) {
		m_clientInfo = info;
		emit clientInfoChanged(m_clientInfo, m_id);
	}
}


void Session::createProjectData(const QString & /*projectId*/)
{
}


void Session::removeProjectData(const QString & /*projectId*/)
{
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
		setState(UnconnectedState);
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

	QList<QDomElement> nodes;
	for (int i = 0; i < reply.childNodes().count(); ++i) {
		QDomNode projectNode = reply.childNodes().at(i);
		if (projectNode.isElement()) {
			nodes.append(projectNode.toElement());
		}
	}
	processProjectNodes(nodes);
}


void Session::processHostInfo(const QByteArray &data)
{
	QDomNode reply = getReply(data);
	if (reply.nodeName() != "host_info") {
		triggerXmlError(reply);
		return;
	}

	HostInfo info(reply.toElement());
	if (info != m_hostInfo) {
		m_hostInfo = info;
		emit hostInfoChanged(m_hostInfo, m_id);
	}
}


void Session::processState(const QByteArray &data)
{
	QDomNode reply = getReply(data);
	if (reply.nodeName() != "client_state") {
		triggerXmlError(reply);
		return;
	}

	processClientInfo(reply.toElement());
}


void Session::processFileTransfers(const QByteArray &data)
{
	QDomNode reply = getReply(data);
	if (reply.nodeName() != "file_transfers") {
		triggerXmlError(reply);
		return;
	}

	// Zoznam kľúčov prenosov súborov, v cykle sa odstraňujú položky a ostanú len
	// tie, ktoré od poslednej aktualizácie zmizli.
	QSet<QPair<QString, QString> > transfersSet = QSet<QPair<QString, QString> >::fromList(m_transfers.keys());

	for (int i = 0; i < reply.childNodes().count(); ++i) {
		QDomNode node = reply.childNodes().at(i);
		if (!node.isElement()) {
			continue;
		}

		FileTransferInfo ftInfo(node.toElement());
		// Už existuje, aktualizujeme
		if (transfersSet.contains(ftInfo.primaryKey())) {
			if (ftInfo != m_transfers.find(ftInfo.primaryKey()).value()) {
				m_transfers.insert(ftInfo.primaryKey(), ftInfo);
				emit transferChanged(ftInfo, m_id);
			}
			transfersSet.remove(ftInfo.primaryKey());
		}
		// Neexistuje, pridávame
		else {
			m_transfers.insert(ftInfo.primaryKey(), ftInfo);
			emit transferAdded(ftInfo, m_id);
		}
	}

	// Odstránime zostávajúce prenosy súborov
	QPair<QString, QString> trans;
	foreach(trans, transfersSet) {
		QMap<QPair<QString, QString>, FileTransferInfo >::iterator fileTransfer = m_transfers.find(trans);
		FileTransferInfo info = fileTransfer.value();
		m_transfers.erase(fileTransfer);
		emit transferRemoved(info, m_id);
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

