/*
 * =====================================================================================
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
 * =====================================================================================
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <QtCore/QObject>
#include <QtCore/QMap>

namespace InfoBoinc {
	class Session;
} /* end of namespace InfoBoinc */

/**
 * \class Engine
 */
class Engine: public QObject
{
Q_OBJECT
public:
	~Engine();

	static Engine &getInstance();
	int initialize();
	void addSession(const QString &host, quint16 port, const QString &password);

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
	typedef QMap<quint32, InfoBoinc::Session *> TSessions;
	TSessions m_sessions;
}; /* -----  end of class Engine  ----- */

#endif    /* ----- #ifndef ENGINE_H  ----- */

