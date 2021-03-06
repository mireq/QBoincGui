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


QList<QString> Session::projects() const
{
	return m_projects.keys();
}


const ProjectInfo &Session::project(const QString &masterURL) const
{
	QMap<QString, ProjectInfo>::const_iterator info = m_projects.find(masterURL);
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


ResultInfo Session::result(const QString &masterURL, const QString &name) const
{
	if (m_results.contains(masterURL)) {
		QMap<QString, ResultInfo>::const_iterator resultIter = m_results[masterURL].find(name);
		if (resultIter != m_results[masterURL].end()) {
			return resultIter.value();
		}
	}
	return ResultInfo();
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


void Session::processProjects(const QList<ProjectInfo> &projects)
{
	QSet<QString> projectsSet = QSet<QString>::fromList(m_projects.keys());
	QList<QString> added;
	QList<QString> changed;

	// Zistenie zmien v projektoch a ich aktualizácia
	foreach(const ProjectInfo &project, projects) {
		const QString masterURL = project.primaryKey();

		QSet<QString>::iterator projectIterator = projectsSet.find(project.primaryKey());

		// Projekt ešte nie je v zozname projektov
		if (projectIterator == projectsSet.end()) {
			m_projects.insert(masterURL, project);
			createProjectData(masterURL);
			added.append(masterURL);
		}
		else {
			if (project != this->project(masterURL)) {
				m_projects.insert(masterURL, project);
				changed.append(masterURL);
			}
			projectsSet.erase(projectIterator);
		}
	}

	if (!added.isEmpty()) {
		emit projectsAdded(added, m_id);
	}
	if (!projectsSet.isEmpty()) {
		emit projectsRemoved(projectsSet.toList(), m_id);
	}
	if (!changed.isEmpty()) {
		emit projectsChanged(changed, m_id);
	}

	foreach(const QString &masterURL, projectsSet) {
		m_projects.remove(masterURL);
		removeProjectData(masterURL);
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


void Session::processWorkunits(const QMap<QString, QList<QDomElement> > &workunitElements, const QMap<QString, QList<QDomElement> > &resultElements)
{
	QMap<QString, QList<QString> > added;
	QMap<QString, QList<QString> > changed;

	QMap<QString, QMap<QString, WorkunitInfo> > workunits;

	for (QMap<QString, QList<QDomElement> >::const_iterator wuMapIter = workunitElements.begin(); wuMapIter != workunitElements.end(); ++wuMapIter) {
		const QString masterURL = wuMapIter.key();
		foreach(const QDomElement &wuElement, wuMapIter.value()) {
			WorkunitInfo workunit(wuElement);
			workunits[masterURL].insert(workunit.primaryKey(), workunit);
		}
	}

	for (QMap<QString, QList<QDomElement> >::const_iterator resultMapIter = resultElements.begin(); resultMapIter != resultElements.end(); ++resultMapIter) {
		const QString masterURL = resultMapIter.key();
		foreach(const QDomElement &resultElement, resultMapIter.value()) {
			ResultInfo result(resultElement);
			QMap<QString, QMap<QString, WorkunitInfo> >::iterator wuListIterator = workunits.find(masterURL);
			if (wuListIterator != workunits.end()) {
				QMap<QString, WorkunitInfo>::iterator wuIterator  = wuListIterator.value().find(result.wuName());
				if (wuIterator != wuListIterator.value().end()) {
					result.bindWorkunit(wuIterator.value());
					result.bindWorkunit(wuIterator.value());
				}
			}
			if (!m_results[masterURL].contains(result.primaryKey())) {
				added[masterURL].append(result.primaryKey());
			}
			else {
				QMap<QString, ResultInfo>::iterator old = m_results[masterURL].find(result.primaryKey());
				if (old.value() != result) {
					changed[masterURL].append(result.primaryKey());
				}
			}
			m_results[masterURL].insert(result.primaryKey(), result);
		}
	}
/*
	for (QMap<QString, QList<QDomElement> >::const_iterator wuMapIter = workunits.begin(); wuMapIter != workunits.end(); ++wuMapIter) {
		const QString masterURL = wuMapIter.key();
		foreach(const QDomElement &wuElement, wuMapIter.value()) {
			WorkunitInfo workunit(wuElement);
			if (!m_workunits[masterURL].contains(workunit.primaryKey())) {
				added[masterURL].append(workunit.primaryKey());
			}
			else {
				QMap<QString, WorkunitInfo>::iterator old = m_workunits[masterURL].find(workunit.primaryKey());
				if (old.value() != workunit) {
					changed[masterURL].append(workunit.primaryKey());
				}
			}
			m_workunits[masterURL].insert(workunit.primaryKey(), workunit);
		}
	}
*/
	if (!added.empty()) {
		emit resultsAdded(added, m_id);
	}
	if (!changed.empty()) {
		emit resultsChanged(changed, m_id);
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

	QList<ProjectInfo> projects;
	for (int i = 0; i < reply.childNodes().count(); ++i) {
		QDomNode projectNode = reply.childNodes().at(i);
		if (projectNode.isElement()) {
			projects.append(ProjectInfo(projectNode.toElement()));
		}
	}
	processProjects(projects);
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
	QList<ProjectInfo> projects;
	QMap<QString, QList<QDomElement> > workunitNodes;
	QMap<QString, QList<QDomElement> > resultNodes;
	for (int i = 0; i < reply.childNodes().count(); ++i) {
		QDomNode node = reply.childNodes().at(i);
		if (!node.isElement()) {
			continue;
		}
		if (node.nodeName() == "project") {
			projects.append(ProjectInfo(node.toElement()));
		}
		else if (node.nodeName() == "workunit") {
			workunitNodes[projects.last().primaryKey()].append(node.toElement());
		}
		else if (node.nodeName() == "result") {
			resultNodes[projects.last().primaryKey()].append(node.toElement());
		}
	}
	processProjects(projects);
	processWorkunits(workunitNodes, resultNodes);
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

