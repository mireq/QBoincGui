/*
 * =====================================================================================
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
 * =====================================================================================
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
	if (m_uiMaster != 0) {
		m_uiMaster->deleteLater();
		m_uiMaster = 0;
	}

	foreach (Session *session, m_sessions) {
		session->deleteLater();
	}
}


Engine &Engine::getInstance()
{
	static Engine instance;
	return instance;
}


int Engine::initialize()
{
	m_uiMaster = new ui_Advanced;
	return 0;
}


void Engine::addSession(const QString &host, quint16 port, const QString &password)
{
	Session *session = new Session;
	m_sessions[session->id()] = session;
	session->openSession(host, port, password);
}


void Engine::timerEvent(QTimerEvent * /*event*/)
{
	foreach (Session *session, m_sessions) {
		if (session->state() == Session::ConnectedState) {
			session->requestProjectStatus();
		}
	}
}

