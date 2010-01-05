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

#include "Engine.h"
#include <QDebug>

using namespace InfoBoinc;

Engine::Engine()
	: QObject()
{
	startTimer(RefreshRate);
}


Engine::~Engine()
{
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

