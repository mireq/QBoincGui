/*
 * =====================================================================================
 *
 *       Filename:  debug.cpp
 *
 *        Version:  1.0
 *        Created:  01.01.2010 14:42:36
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#include <QtCore/QMutex>
#include <QtCore/QRegExp>
#include <QtGui/QApplication>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "config.h"

#ifdef DEBUG_MSG_COLORING
#define ANSI_FG_BLACK   "\033[0;30m"
#define ANSI_FG_RED     "\033[0;31m"
#define ANSI_FG_GREEN   "\033[0;32m"
#define ANSI_FG_YELLOW  "\033[0;33m"
#define ANSI_FG_BLUE    "\033[0;34m"
#define ANSI_FG_MAGENTA "\033[0;35m"
#define ANSI_FG_CYAN    "\033[0;36m"
#define ANSI_FG_WHITE   "\033[0;37m"
#define ANSI_FG_RESET   "\033[0;37m"
#define ANSI_FG_LIGHTBLACK   "\033[1;30m"
#define ANSI_FG_LIGHTRED     "\033[1;31m"
#define ANSI_FG_LIGHTGREEN   "\033[1;32m"
#define ANSI_FG_LIGHTYELLOW  "\033[1;33m"
#define ANSI_FG_LIGHTBLUE    "\033[1;34m"
#define ANSI_FG_LIGHTMAGENTA "\033[1;35m"
#define ANSI_FG_LIGHTCYAN    "\033[1;36m"
#define ANSI_FG_LIGHTWHITE   "\033[1;37m"

#ifdef DEBUG_MSG_COLORING_256
#define COLOR_MODULE_NAME  "\033[1m\033[38;5;228m"
#define COLOR_MSG_TYPE     "\033[38;5;203m"
#define COLOR_MSG_IN       "\033[38;5;120m"
#define COLOR_MSG_OUT      "\033[38;5;122m"
#define COLOR_DEBUG_MSG    "\033[38;5;105m"
#define COLOR_WARNING_MSG  "\033[38;5;226m"
#define COLOR_CRITICAL_MSG "\033[38;5;217m"
#define COLOR_FATAL_MSG    "\033[38;5;203m"
#else /* ----- #ifdef DEBUG_MSG_COLORING_256  ----- */
#define COLOR_MODULE_NAME  ANSI_FG_LIGHTYELLOW
#define COLOR_MSG_TYPE     ANSI_FG_LIGHTRED
#define COLOR_MSG_IN       ANSI_FG_GREEN
#define COLOR_MSG_OUT      ANSI_FG_LIGHTGREEN
#define COLOR_DEBUG_MSG    ANSI_FG_LIGHTBLUE
#define COLOR_WARNING_MSG  ANSI_FG_LIGHTYELLOW
#define COLOR_CRITICAL_MSG ANSI_FG_LIGHTRED
#define COLOR_FATAL_MSG    ANSI_FG_LIGHTRED
#endif /* ----- #ifdef DEBUG_MSG_COLORING_256  ----- */

void printColorMsg(const char *msg, const char *color) {
	static QMutex mutex;
	QRegExp expr("^<([^>|]*)(?:\\|([^>|]*))?> (.*)");
	QString message = QString::fromUtf8(msg);
	int pos = expr.indexIn(msg);
	if (pos > -1) {
		//fprintf(stderr, "%s\n", msg);
		const QString module  = expr.cap(1);
		const QString type    = expr.cap(2);
		const QString msgBody = expr.cap(3);
		mutex.lock();
		if (type == "In" || type == "Out") {
			fprintf(stderr, COLOR_MODULE_NAME"%s"ANSI_FG_RESET "| " COLOR_MSG_TYPE, module.toUtf8().constData());
			if (type == "In") {
				fprintf(stderr, "Read\n"ANSI_FG_RESET COLOR_MSG_IN);
			}
			else {
				fprintf(stderr, "Write\n"ANSI_FG_RESET COLOR_MSG_OUT);
			}
			fprintf(stderr, "%s\n"ANSI_FG_RESET, msgBody.toUtf8().constData());
		}
		else {
			fprintf(stderr, COLOR_MODULE_NAME"%s"ANSI_FG_RESET, module.toUtf8().constData());
			if (!type.isEmpty()) {
				fprintf(stderr, "| "COLOR_MSG_TYPE"%s"ANSI_FG_RESET": ", type.toUtf8().constData());
			}
			else {
				fprintf(stderr, ": ");
			}
			fprintf(stderr, "%s%s\n"ANSI_FG_RESET, color, msgBody.toUtf8().constData());
		}
		mutex.unlock();
	}
	else {
		fprintf(stderr, "%s%s\n"ANSI_FG_RESET, color, msg);
	}
}

#endif /* ----- #ifdef DEBUG_MSG_COLORING  ----- */

void printDebugMsg(const char *msg);
void printWarningMsg(const char *msg);
void printCriticalMsg(const char *msg);
void printFatalMsg(const char *msg);

void infoBoincMsgHandler(QtMsgType type, const char *msg)
{
	switch(type) {
		case QtDebugMsg:
			printDebugMsg(msg);
			break;
		case QtWarningMsg:
			printWarningMsg(msg);
			break;
		case QtCriticalMsg:
			printCriticalMsg(msg);
			break;
		case QtFatalMsg:
			printFatalMsg(msg);
			abort();
	}
}


void printDebugMsg(const char *msg)
{
#ifdef DEBUG_MSG_COLORING
	printColorMsg(msg, COLOR_DEBUG_MSG);
#else /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
	fprintf(stderr, "%s\n", msg);
#endif /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
}


void printWarningMsg(const char *msg)
{
#ifdef DEBUG_MSG_COLORING
	printColorMsg(msg, COLOR_WARNING_MSG);
#else /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
	fprintf(stderr, "%s\n", msg);
#endif /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
}


void printCriticalMsg(const char *msg)
{
#ifdef DEBUG_MSG_COLORING
	printColorMsg(msg, COLOR_CRITICAL_MSG);
#else /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
	fprintf(stderr, "%s\n", msg);
#endif /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
}


void printFatalMsg(const char *msg)
{
#ifdef DEBUG_MSG_COLORING
	printColorMsg(msg, COLOR_FATAL_MSG);
#else /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
	fprintf(stderr, "%s\n", msg);
#endif /* ----- #ifdef DEBUG_MSG_COLORING  ----- */
}


MsgInfo debugMsgInfo(const QString &module, const QString &msgType)
{
	return MsgInfo(module, msgType);
}


MsgInfo debugMsgInfo(QObject *module, const QString &msgType)
{
	return MsgInfo(module, msgType);
}


QDebug operator<<(QDebug dbg, const MsgInfo &mi)
{
	QString mtype;
	if (!mi.msgType().isEmpty()) {
		mtype = QString("|") + mi.msgType();
	}
	dbg << QString("<%1%2>").arg(mi.module(), mtype).toUtf8().constData();
	return dbg;
}

