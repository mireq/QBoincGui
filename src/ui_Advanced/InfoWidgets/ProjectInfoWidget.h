/*
 * =====================================================================
 *
 *       Filename:  ProjectInfoWidget.h
 *
 *        Version:  1.0
 *        Created:  21.01.2010 18:13:44
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef PROJECTINFOWIDGET_H
#define PROJECTINFOWIDGET_H

#include "Session.h"
#include "ui_Advanced/InfoWidget.h"

namespace Ui {
	class ProjectInfoWidget;
}

namespace ui_AdvancedNS {

class ProjectInfoWidget: public InfoWidget
{
Q_OBJECT
public:
	ProjectInfoWidget(InfoBoinc::Session *session, const QString &masterURL, QWidget *parent = 0);
	~ProjectInfoWidget();
	virtual QString widgetTitle() const;

private slots:
	void handleProjectsUpdate(const QList<QString> &projects);

private:
	void updateProjectInfo(const InfoBoinc::ProjectInfo &projectInfo);

private:
	Ui::ProjectInfoWidget *ui;
	InfoBoinc::Session *m_sessionPtr;
	QString m_masterURL;
	QString m_projectName;
}; /* -----  end of class ProjectInfoWidget  ----- */


} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef PROJECTINFOWIDGET_H  ----- */

