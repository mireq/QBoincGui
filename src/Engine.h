/*
 * =====================================================================
 *
 *       Filename:  Engine.h
 *
 *        Version:  1.0
 *        Created:  04.01.2010 09:02:53
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include "Session.h"

class ui_Master;

/**
 * \class Engine
 */
class Engine: public QObject
{
Q_OBJECT
public:
	~Engine();

	static Engine &getInstance();
	void initializeUi();
	int initialize();
	InfoBoinc::Session::IdType addSession(const QString &host, quint16 port, const QString &password, const QString &directory = QString());
	InfoBoinc::Session *session(InfoBoinc::Session::IdType id);

signals:
	void sessionAdded(InfoBoinc::Session::IdType id);

private:
	virtual void timerEvent(QTimerEvent *event);

// Zákaz konštruktorov (singleton)
private:
	static const unsigned long RefreshRate = 2000;

	Engine();
	/*Engine(const Engine &): QObject() {};
	Engine & operator= (const Engine &) { return *this; };*/
	Q_DISABLE_COPY(Engine)
private:
	/* ====================  DATA MEMBERS  ==================== */
	typedef QMap<InfoBoinc::Session::IdType, InfoBoinc::Session *> TSessions;
	TSessions m_sessions;
	ui_Master *m_uiMaster;
}; /* -----  end of class Engine  ----- */

#endif    /* ----- #ifndef ENGINE_H  ----- */

