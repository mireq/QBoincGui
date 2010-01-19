/*
 * =====================================================================
 *
 *       Filename:  SessionInfoWidget.h
 *
 *        Version:  1.0
 *        Created:  18.01.2010 10:51:12
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef SESSIONINFOWIDGET_H
#define SESSIONINFOWIDGET_H

#include "ui_Advanced/InfoWidget.h"
#include "Session.h"
#include <QtGui/QWidget>

namespace Ui {
	class SessionInfoWidget;
}

namespace ui_AdvancedNS {

class SessionInfoWidget: public InfoWidget
{
Q_OBJECT
public:
	SessionInfoWidget(InfoBoinc::Session *session, QWidget *parent = 0);
	~SessionInfoWidget();

private slots:
	void updateHostInfo(const InfoBoinc::HostInfo &hostInfo);
	void updateClientInfo(const InfoBoinc::ClientInfo &clientInfo);
	void updateSessionState(InfoBoinc::Session::State state);

private:
	Ui::SessionInfoWidget *ui;
	InfoBoinc::Session *m_sessionPtr;
}; /* -----  end of class SessionInfoWidget  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef SESSIONINFOWIDGET_H  ----- */

