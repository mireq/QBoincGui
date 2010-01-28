/*
 * =====================================================================
 *
 *       Filename:  TransfersInfoWidget.cpp
 *
 *        Version:  1.0
 *        Created:  20.01.2010 20:17:19
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtGui/QHeaderView>
#include "debug.h"
#include "GuiIcon.h"
#include "PresentationSingleton.h"
#include "ui_Advanced/ProgressItemDelegate.h"
#include "TransfersInfoWidget.h"
#include "ui_TransfersInfoWidget.h"

namespace ui_AdvancedNS {

const uint TransfersInfoWidget::UpdateInterval = 500;

TransfersInfoWidget::TransfersInfoWidget(InfoBoinc::Session *session, QWidget *parent):
	InfoWidget(parent),
	ui(new Ui::TransfersInfoWidget),
	m_sessionPtr(session)
{
	ui->setupUi(this);
	ui->transfersList->header()->setResizeMode(IconColumn, QHeaderView::ResizeToContents);
	ui->transfersList->headerItem()->setData(IconColumn, Qt::DisplayRole, "");
	ui->transfersList->setItemDelegateForColumn(ProgressColumn, new ProgressItemDelegate);

	const QList<InfoBoinc::FileTransferInfo> transfers = session->transfers();
	foreach (const InfoBoinc::FileTransferInfo &info, transfers) {
		addTransfer(info);
	}

	connect(session, SIGNAL(transferAdded(InfoBoinc::FileTransferInfo,InfoBoinc::Session::IdType)), SLOT(addTransfer(InfoBoinc::FileTransferInfo)));
	connect(session, SIGNAL(transferRemoved(InfoBoinc::FileTransferInfo,InfoBoinc::Session::IdType)), SLOT(removeTransfer(InfoBoinc::FileTransferInfo)));
	connect(session, SIGNAL(transferChanged(InfoBoinc::FileTransferInfo,InfoBoinc::Session::IdType)), SLOT(changeTransfer(InfoBoinc::FileTransferInfo)));
	startTimer(UpdateInterval);
}

TransfersInfoWidget::~TransfersInfoWidget()
{
	delete ui;
}

void TransfersInfoWidget::timerEvent(QTimerEvent * /*event*/)
{
	m_sessionPtr->requestFileTransfers();
	foreach (QTreeWidgetItem *item, m_items) {
		updateStatus(item);
	}
}

void TransfersInfoWidget::addTransfer(const InfoBoinc::FileTransferInfo &info)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	if (info.generatedLocally()) {
		item->setData(0, Qt::DecorationRole, GuiIcon("uploading", "status"));
	}
	else {
		item->setData(0, Qt::DecorationRole, GuiIcon("downloading", "status"));
	}
	setItemProperties(info, item);
	ui->transfersList->addTopLevelItem(item);
	m_items.insert(info.primaryKey(), item);
}

void TransfersInfoWidget::removeTransfer(const InfoBoinc::FileTransferInfo &info)
{
	QMap<QPair<QString, QString>, QTreeWidgetItem *>::iterator itemIterator = m_items.find(info.primaryKey());
	if (itemIterator == m_items.end()) {
		qWarning() << debugMsgInfo(this) << "Item not found";
		return;
	}

	int index = ui->transfersList->indexOfTopLevelItem(itemIterator.value());
	ui->transfersList->takeTopLevelItem(index);
	delete itemIterator.value();
	m_items.erase(itemIterator);
}

void TransfersInfoWidget::changeTransfer(const InfoBoinc::FileTransferInfo &info)
{
	QMap<QPair<QString, QString>, QTreeWidgetItem *>::iterator itemIterator = m_items.find(info.primaryKey());
	if (itemIterator == m_items.end()) {
		qWarning() << debugMsgInfo(this) << "Item not found";
		return;
	}

	setItemProperties(info, itemIterator.value());
}

void TransfersInfoWidget::setItemProperties(const InfoBoinc::FileTransferInfo &info, QTreeWidgetItem *item)
{
	double progress = 0;
	if (info.nBytes() != 0) {
		progress = 100.0 * info.bytesXferred() / info.nBytes();
	}
	QString sizeString = PresentationSingleton::getInstance().byteCountToString(info.bytesXferred());
	sizeString += " / " + PresentationSingleton::getInstance().byteCountToString(info.nBytes());
	QString speedString = PresentationSingleton::getInstance().byteCountToString(info.xferSpeed()) + "/s";

	item->setData(ProjectColumn,  Qt::DisplayRole,  QVariant(info.projectName()));
	item->setData(NameColumn,     Qt::DisplayRole,  QVariant(info.name()));
	item->setData(ProgressColumn, Qt::DisplayRole,  QVariant(progress));
	item->setData(SizeColumn,     Qt::DisplayRole,  QVariant(sizeString));
	item->setData(SpeedColumn,    Qt::DisplayRole,  QVariant(speedString));
	item->setData(StatusColumn,   StatusInfo,       info.status());
	item->setData(StatusColumn,   ActiveInfo,       info.isXferActive());
	item->setData(StatusColumn,   LocallyInfo,      info.generatedLocally());
	item->setData(StatusColumn,   FirstRequestInfo, info.firstRequestTime());
	item->setData(StatusColumn,   NextRequestInfo,  info.nextRequestTime());
	updateStatus(item);
}

void TransfersInfoWidget::updateStatus(QTreeWidgetItem *item)
{
	QString statusMessage;
	int status = item->data(StatusColumn, StatusInfo).toInt();
	bool active = item->data(StatusColumn, ActiveInfo).toBool();
	bool generatedLocally = item->data(StatusColumn, LocallyInfo).toBool();

	if (item->data(StatusColumn, NextRequestInfo).toDateTime().toTime_t() != 0) {
		uint nextReq = item->data(StatusColumn, NextRequestInfo).toDateTime().toTime_t();
		uint now = QDateTime::currentDateTime().toTime_t();
		if (nextReq > now && item->data(StatusColumn, FirstRequestInfo).toDateTime().isValid()) {
			statusMessage = tr("Retry in") + " " + PresentationSingleton::getInstance().timeIntervalToString(nextReq - now);
		}
		else {
			statusMessage = tr("Suspended");
		}
	}
	else if (status == InfoBoinc::FileTransferInfo::ErrorDownloadStatus) {
		statusMessage = tr("Download failed");
	}
	else if (status == InfoBoinc::FileTransferInfo::ErrorUploadStatus) {
		statusMessage = tr("Upload failed");
	}
	else if (active) {
		statusMessage = generatedLocally ? tr("Uploading") : tr("Downloading");
	}
	else {
		statusMessage = generatedLocally ? tr("Upload pending") : tr("Download pending");
	}
	item->setData(StatusColumn, Qt::DisplayRole, statusMessage);
}

} /* end of namespace ui_AdvancedNS */

