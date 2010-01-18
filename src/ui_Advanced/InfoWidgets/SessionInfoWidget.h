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

using namespace InfoBoinc;

class SessionInfoWidget: public InfoWidget
{
Q_OBJECT
public:
	SessionInfoWidget(Session *session, QWidget *parent = 0);
	~SessionInfoWidget();
private:
	Ui::SessionInfoWidget *ui;
}; /* -----  end of class SessionInfoWidget  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef SESSIONINFOWIDGET_H  ----- */

