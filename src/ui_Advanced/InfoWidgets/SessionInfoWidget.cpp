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

SessionInfoWidget::SessionInfoWidget(InfoBoinc::Session *session, QWidget *parent):
	InfoWidget(parent),
	ui(new Ui::SessionInfoWidget),
	m_sessionPtr(session)
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

	updateHostInfo(m_sessionPtr->hostInfo());
	updateClientInfo(m_sessionPtr->clientInfo());
	updateSessionState(m_sessionPtr->state());
	connect(session, SIGNAL(hostInfoChanged(const InfoBoinc::HostInfo &, InfoBoinc::Session::IdType)), SLOT(updateHostInfo(const InfoBoinc::HostInfo &)));
	connect(session, SIGNAL(stateChanged(InfoBoinc::Session::State, InfoBoinc::Session::IdType)), SLOT(updateSessionState(InfoBoinc::Session::State)));
	connect(session, SIGNAL(clientInfoChanged(const InfoBoinc::ClientInfo &, InfoBoinc::Session::IdType)), SLOT(updateClientInfo(const InfoBoinc::ClientInfo &)));

	// Ak nie je host info cachované vyžiadame si ho
	if (m_sessionPtr->hostInfo().isNull()) {
		session->requestHostInfo();
	}
}


SessionInfoWidget::~SessionInfoWidget()
{
	delete ui;
}


void SessionInfoWidget::updateHostInfo(const InfoBoinc::HostInfo &hostInfo)
{
	if (hostInfo.isNull()) {
		ui->hostInfoGroup->hide();
	}
	else {
		ui->domainNameLabel->setText(hostInfo.domainName());
		ui->osNameLabel->setText(hostInfo.osName());
		ui->osVersionLabel->setText(hostInfo.osVersion());
		ui->hostInfoGroup->show();
	}
}


void SessionInfoWidget::updateClientInfo(const InfoBoinc::ClientInfo &clientInfo)
{
	if (clientInfo.isNull()) {
		ui->platformLabel->setText("");
		ui->clientVersionLabel->setText("");
	}
	else {
		ui->platformLabel->setText(clientInfo.platformName());
		ui->clientVersionLabel->setText(clientInfo.coreClientVersion());
	}
}


void SessionInfoWidget::updateSessionState(InfoBoinc::Session::State state)
{
	switch (state) {
		case InfoBoinc::Session::UnconnectedState:
			ui->clientStateLabel->setText(tr("Unconnected"));
			break;
		case InfoBoinc::Session::ConnectingState:
			ui->clientStateLabel->setText(tr("Connecting"));
			break;
		case InfoBoinc::Session::ConnectedState:
			ui->clientStateLabel->setText(tr("Connected"));
			break;
		case InfoBoinc::Session::DisconnectingState:
			ui->clientStateLabel->setText(tr("Disconnecting"));
			break;
	}
}

} /* end of namespace ui_AdvancedNS */

