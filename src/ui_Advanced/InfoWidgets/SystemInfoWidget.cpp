/*
 * =====================================================================
 *
 *       Filename:  SystemInfoWidget.cpp
 *
 *        Version:  1.0
 *        Created:  20.01.2010 12:14:45
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtCore/QLocale>
#include "PresentationSingleton.h"
#include "SystemInfoWidget.h"
#include "ui_SystemInfoWidget.h"

namespace ui_AdvancedNS {

SystemInfoWidget::SystemInfoWidget(InfoBoinc::Session *session, QWidget *parent):
	InfoWidget(parent),
	ui(new Ui::SystemInfoWidget)
{
	ui->setupUi(this);

	updateHostInfo(session->hostInfo());
	connect(session, SIGNAL(hostInfoChanged(const InfoBoinc::HostInfo &, InfoBoinc::Session::IdType)), SLOT(updateHostInfo(const InfoBoinc::HostInfo &)));
	if (session->hostInfo().isNull()) {
		session->requestHostInfo();
	}
}


SystemInfoWidget::~SystemInfoWidget()
{
	delete ui;
}


void SystemInfoWidget::updateHostInfo(const InfoBoinc::HostInfo &hostInfo)
{
	if (hostInfo.isNull()) {
		ui->processorGroup->hide();
		ui->memoryGroup->hide();
		ui->diskGroup->hide();
	}
	else {
		QLocale locale = QLocale::system();
		ui->pCpusLabel->setText(QString::number(hostInfo.pCpus()));
		ui->pVendorLabel->setText(hostInfo.pVendor());
		ui->pModelLabel->setText(hostInfo.pModel());
		ui->pFeaturesLabel->setText(hostInfo.pFeatures());
		ui->pFpopsLabel->setText(locale.toString(hostInfo.pFpops() / 1000000.0, 'f', 2) + " " + tr("million ops/sec"));
		ui->pIopsLabel->setText(locale.toString(hostInfo.pIops() / 1000000.0, 'f', 2) + " " + tr("million ops/sec"));

		ui->mNbytesLabel->setText(PresentationSingleton::getInstance().byteCountToString(hostInfo.mNbytes()));
		ui->mCacheLabel->setText(PresentationSingleton::getInstance().byteCountToString(hostInfo.mCache()));
		ui->mSwapLabel->setText(PresentationSingleton::getInstance().byteCountToString(hostInfo.mSwap()));

		ui->dTotalLabel->setText(PresentationSingleton::getInstance().byteCountToString(hostInfo.dTotal()));
		ui->dFreeLabel->setText(PresentationSingleton::getInstance().byteCountToString(hostInfo.dFree()));

		ui->processorGroup->show();
		ui->memoryGroup->show();
		ui->diskGroup->show();
	}
}


} /* end of namespace ui_AdvancedNS */

