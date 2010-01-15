/*
 * =====================================================================
 *
 *       Filename:  ui_Master.h
 *
 *        Version:  1.0
 *        Created:  06.01.2010 19:46:00
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef UI_MASTER_H
#define UI_MASTER_H

#include <QtCore/QObject>
#include "Session.h"

/**
 * \class ui_Master
 */
class ui_Master: public QObject
{
Q_OBJECT
public slots:
	virtual void addSession(InfoBoinc::Session::IdType id) = 0;
}; /* -----  end of class ui_Master  ----- */

#endif    /* ----- #ifndef UI_MASTER_H  ----- */

