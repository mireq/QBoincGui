/*
 * =====================================================================
 *
 *       Filename:  TransfersInfoWidget.h
 *
 *        Version:  1.0
 *        Created:  20.01.2010 20:17:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef TRANSFERSINFOWIDGET_H
#define TRANSFERSINFOWIDGET_H

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtGui/QTreeWidgetItem>
#include "Session.h"
#include "ui_Advanced/InfoWidget.h"

namespace Ui {
	class TransfersInfoWidget;
}

namespace ui_AdvancedNS {

class TransfersInfoWidget: public InfoWidget
{
Q_OBJECT
public:
	TransfersInfoWidget(InfoBoinc::Session *session, QWidget *parent = 0);
	~TransfersInfoWidget();

protected:
	void timerEvent(QTimerEvent *event);

private slots:
	void addTransfer(const InfoBoinc::FileTransferInfo &info);
	void removeTransfer(const InfoBoinc::FileTransferInfo &info);
	void changeTransfer(const InfoBoinc::FileTransferInfo &info);

private:
	enum ItemInfo {
		StatusInfo = Qt::UserRole,
		ActiveInfo,
		LocallyInfo,
		NextRequestInfo
	};

	enum Column {
		IconColumn,
		ProjectColumn,
		NameColumn,
		ProgressColumn,
		SizeColumn,
		SpeedColumn,
		StatusColumn
	};

	void setItemProperties(const InfoBoinc::FileTransferInfo &info, QTreeWidgetItem *item);
	void updateStatus(QTreeWidgetItem *item);

private:
	Ui::TransfersInfoWidget *ui;
	InfoBoinc::Session *m_sessionPtr;
	QMap<QPair<QString, QString>, QTreeWidgetItem *> m_items;
	static const uint UpdateInterval;
}; /* -----  end of class TransfersInfoWidget  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef TRANSFERSINFOWIDGET_H  ----- */

