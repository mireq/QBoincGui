/*
 * =====================================================================
 *
 *       Filename:  ProjectInfoWidget.cpp
 *
 *        Version:  1.0
 *        Created:  21.01.2010 18:12:51
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtGui/QTextDocument>
#include "PresentationSingleton.h"
#include "ProjectInfoWidget.h"
#include "ui_ProjectInfoWidget.h"

namespace ui_AdvancedNS {

ProjectInfoWidget::ProjectInfoWidget(InfoBoinc::Session *session, const QString &masterURL, QWidget *parent):
	InfoWidget(parent),
	ui(new Ui::ProjectInfoWidget),
	m_sessionPtr(session),
	m_masterURL(masterURL)
{
	ui->setupUi(this);
	updateProjectInfo(session->project(masterURL));
	m_projectName = session->project(masterURL).projectName();
	connect(session, SIGNAL(projectsChanged(QList<QString>, InfoBoinc::Session::IdType)), SLOT(handleProjectsUpdate(QList<QString>)));
}

ProjectInfoWidget::~ProjectInfoWidget()
{
	delete ui;
}

QString ProjectInfoWidget::widgetTitle() const
{
	return windowTitle() + " - " + m_projectName;
}

void ProjectInfoWidget::handleProjectsUpdate(const QList<QString> &projects)
{
	if (projects.contains(m_masterURL)) {
		updateProjectInfo(m_sessionPtr->project(m_masterURL));
	}
}

void ProjectInfoWidget::updateProjectInfo(const InfoBoinc::ProjectInfo &projectInfo)
{
	QString escURL = Qt::escape(projectInfo.masterURL());
	QString urlStr = QString("<a href=\"%1\">%1</a>").arg(escURL);
	ui->projectNameLabel->setText(projectInfo.projectName());
	ui->masterURLLabel->setText(urlStr);
	ui->userNameLabel->setText(projectInfo.userName());
	ui->teamNameLabel->setText(projectInfo.teamName());

	ui->userTotalLabel->setText(PresentationSingleton::getInstance().doubleToString(projectInfo.userTotalCredit(), 'f', 1));
	ui->userAverageLabel->setText(PresentationSingleton::getInstance().doubleToString(projectInfo.userExpavgCredit(), 'f', 2));

	ui->hostTotalLabel->setText(PresentationSingleton::getInstance().doubleToString(projectInfo.hostTotalCredit(), 'f', 1));
	ui->hostAverageLabel->setText(PresentationSingleton::getInstance().doubleToString(projectInfo.hostExpavgCredit(), 'f', 2));
}

} /* end of namespace ui_AdvancedNS */

