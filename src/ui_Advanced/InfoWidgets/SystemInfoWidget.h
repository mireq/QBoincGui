/*
 * =====================================================================
 *
 *       Filename:  SystemInfoWidget.h
 *
 *        Version:  1.0
 *        Created:  20.01.2010 12:14:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef SYSTEMINFOWIDGET_H
#define SYSTEMINFOWIDGET_H

#include "ui_Advanced/InfoWidget.h"
#include "Session.h"

namespace Ui {
	class SystemInfoWidget;
}

namespace ui_AdvancedNS {

class SystemInfoWidget: public InfoWidget
{
Q_OBJECT
public:
	SystemInfoWidget(InfoBoinc::Session *session, QWidget *parent = 0);
	~SystemInfoWidget();

private slots:
	void updateHostInfo(const InfoBoinc::HostInfo &hostInfo);

private:
	Ui::SystemInfoWidget *ui;
}; /* -----  end of class SystemInfoWidget  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef SYSTEMINFOWIDGET_H  ----- */

