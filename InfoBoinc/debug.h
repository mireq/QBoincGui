/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *        Version:  1.0
 *        Created:  01.01.2010 15:11:10
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <QtCore/QDebug>

class MsgInfo {
public:
	MsgInfo(QObject *module, const QString &msgType): m_module(module->metaObject()->className()), m_msgType(msgType) {};
	MsgInfo(const QString &module, const QString &msgType):  m_module(module), m_msgType(msgType) {};
	void setModule(QObject *module) { m_module = module->metaObject()->className(); };
	void setModule(const QString &module) { m_module = module; };
	void setMsgType(const QString &msgType) { m_msgType = msgType; };
	const QString &module() const { return m_module; };
	const QString &msgType() const { return m_msgType; };
private:
	QString m_module;
	QString m_msgType;
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const MsgInfo &mi);
#endif

void infoBoincMsgHandler(QtMsgType type, const char *msg);
MsgInfo debugMsgInfo(QObject *module, const QString &msgType = QString());
MsgInfo debugMsgInfo(const QString &module, const QString &msgType = QString());

#endif    /* ----- #ifndef DEBUG_H  ----- */

