/*
 * =====================================================================
 *
 *       Filename:  SessionInfoWidget.cpp
 *
 *        Version:  1.0
 *        Created:  18.01.2010 10:50:41
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "SessionInfoWidget.h"
#include "ui_SessionInfoWidget.h"

namespace ui_AdvancedNS {

SessionInfoWidget::SessionInfoWidget(Session *session, QWidget *parent):
	InfoWidget(parent),
	ui(new Ui::SessionInfoWidget)
{
	ui->setupUi(this);

	QString isLocalLabelText = tr("No");
	QString directoryLabelText = "---";
	if (session->isLocal()) {
		isLocalLabelText = tr("Yes");
		directoryLabelText = session->directory();
	}
	ui->hostLabel->setText(session->host());
	ui->portLabel->setText(QString::number(session->port()));
	ui->localClientLabel->setText(isLocalLabelText);
	ui->directoryLabel->setText(directoryLabelText);
}

SessionInfoWidget::~SessionInfoWidget()
{
	delete ui;
}

} /* end of namespace ui_AdvancedNS */

