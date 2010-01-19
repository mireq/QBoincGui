/*
 * =====================================================================
 *
 *       Filename:  Engine.cpp
 *
 *        Version:  1.0
 *        Created:  04.01.2010 09:02:50
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtCore/QStringList>
#include <QtGui/QApplication>
#include "Session.h"
#include "ui_Advanced/ui_Advanced.h"
#include "Engine.h"

using namespace InfoBoinc;

Engine::Engine():
	QObject(),
	m_uiMaster(0)
{
	startTimer(RefreshRate);
}


Engine::~Engine()
{
}


Engine &Engine::getInstance()
{
	static Engine instance;
	return instance;
}


void Engine::initializeUi()
{
	m_uiMaster = new ui_Advanced;
	connect(this, SIGNAL(sessionAdded(InfoBoinc::Session::IdType)), m_uiMaster, SLOT(addSession(InfoBoinc::Session::IdType)));
}


int Engine::initialize()
{
	initializeUi();
	return 0;
}


void Engine::deinitialize()
{
	if (m_uiMaster != 0) {
		delete m_uiMaster;
		m_uiMaster = 0;
	}

	foreach (Session *session, m_sessions) {
		session->disconnect();
		delete session;
	}
}


Session::IdType Engine::addSession(const QString &host, quint16 port, const QString &password, const QString &directory)
{
	Session *session = new Session;
	connect(session, SIGNAL(stateChanged(InfoBoinc::Session::State, InfoBoinc::Session::IdType)), SLOT(onSessionStateChanged(InfoBoinc::Session::State, InfoBoinc::Session::IdType)));
	m_sessions[session->id()] = session;
	session->setDirectory(directory);
	session->openSession(host, port, password);
	emit sessionAdded(session->id());
	return session->id();
}


InfoBoinc::Session *Engine::session(InfoBoinc::Session::IdType id)
{
	return m_sessions[id];
}


void Engine::onSessionStateChanged(InfoBoinc::Session::State state, InfoBoinc::Session::IdType id)
{
	Session *session = m_sessions[id];
	if (state == InfoBoinc::Session::ConnectedState) {
		session->requestState();
	}
}


void Engine::timerEvent(QTimerEvent * /*event*/)
{
	foreach (Session *session, m_sessions) {
		if (session->state() == Session::ConnectedState) {
			session->requestProjectStatus();
		}
	}
}

