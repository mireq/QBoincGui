/*
 * =====================================================================
 *
 *       Filename:  MainWindow.cpp
 *
 *        Version:  1.0
 *        Created:  07.01.2010 10:42:57
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtGui/QTextDocument>
#include "AddClientWizard.h"
#include "CoreBoincPlugin.h"
#include "Engine.h"
#include "InfoWidget.h"
#include "Session.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace ui_AdvancedNS {

MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_coreBoincPlugin = new CoreBoincPlugin(ui->boincTree, this);
	m_currentInfoWidget = 0;

	// Nastavenie veľkého titulku
	QFont font = ui->pageTitleLabel->font();
	font.setPointSize(font.pointSize() * 2);
	font.setWeight(QFont::Bold);
	ui->pageTitleLabel->setFont(font);
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::addSession(InfoBoinc::Session::IdType id)
{
	ui->boincTree->addSession(id);
	m_coreBoincPlugin->registerSession(id);
}


void MainWindow::on_actionAddClient_triggered()
{
	AddClientWizard wizard;
	if (wizard.exec() == QDialog::Accepted) {
		QString host = wizard.field("host").toString();
		quint16 port = wizard.field("port").toUInt();
		QString pass = wizard.field("password").toString();
		QString directory;
		if (wizard.field("isLocal").toBool()) {
			directory = wizard.field("directory").toString();
		}
		Engine::getInstance().addSession(host, port, pass, directory);
	}
}


void MainWindow::on_boincTree_currentItemChanged(QTreeWidgetItem *current)
{
	InfoWidget *widget = 0;
	if (current != 0) {
		widget = m_coreBoincPlugin->createInfoWidget(current);
	}
	setInfoWidget(widget);
}


void MainWindow::setInfoWidget(InfoWidget *widget)
{
	QWidget *old = ui->stackedWidget->currentWidget();
	if (widget != 0) {
		ui->stackedWidget->insertWidget(0, widget);
		ui->stackedWidget->setCurrentIndex(0);
		ui->pageTitleLabel->setText(widget->widgetTitle());
		QSize pixmapSize = widget->widgetIcon().actualSize(QSize(64, 64));
		if (pixmapSize.isValid()) {
			ui->pageIconLabel->setPixmap(widget->widgetIcon().pixmap(pixmapSize));
		}
		else {
			ui->pageIconLabel->setPixmap(QPixmap());
		}
	}
	else {
		ui->pageTitleLabel->setText("");
		ui->pageIconLabel->setPixmap(QPixmap());
	}
	if (old != 0) {
		ui->stackedWidget->removeWidget(old);
		old->deleteLater();
	}
}

} /* end of namespace ui_AdvancedNS */

