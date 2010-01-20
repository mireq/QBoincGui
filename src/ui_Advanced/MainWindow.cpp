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
	ui(new Ui::MainWindow),
	m_tabWidget(0)
{
	ui->setupUi(this);
	m_coreBoincPlugin = new CoreBoincPlugin(ui->boincTree, this);

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
	QList<InfoWidget *> widgets;
	if (current != 0) {
		widgets = m_coreBoincPlugin->createInfoWidgets(current);
	}
	setInfoWidgets(widgets);
}


void MainWindow::setInfoWidgets(QList<InfoWidget *> widgets)
{
	/*
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
	*/

	QTabWidget *newTabs = 0;
	if (widgets.count() != 0) {
		if (widgets.count() == 1) {
			InfoWidget *widget = widgets.first();
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
			newTabs = new QTabWidget;
			ui->stackedWidget->insertWidget(0, newTabs);
			foreach(InfoWidget *infoWidget, widgets) {
				newTabs->addTab(infoWidget, infoWidget->widgetIcon(), infoWidget->widgetTitle());
			}
		}
		ui->stackedWidget->setCurrentIndex(0);
	}
	else {
		ui->pageTitleLabel->setText("");
		ui->pageIconLabel->setPixmap(QPixmap());
	}

	if (m_tabWidget != 0) {
		ui->stackedWidget->removeWidget(m_tabWidget);
		m_tabWidget->disconnect();
		m_tabWidget->deleteLater();
		foreach(InfoWidget *infoWidget, m_infoWidgets) {
			infoWidget->deleteLater();
		}
	}
	else {
		foreach(InfoWidget *infoWidget, m_infoWidgets) {
			ui->stackedWidget->removeWidget(infoWidget);
			infoWidget->deleteLater();
		}
	}

	m_tabWidget = newTabs;
	m_infoWidgets = widgets;
	if (newTabs != 0) {
		connect(newTabs, SIGNAL(currentChanged(int)), SLOT(updateTitleInfo(int)));
		updateTitleInfo(newTabs->currentIndex());
	}
}


void MainWindow::updateTitleInfo(int newTabIdx)
{
	InfoWidget *widget = m_infoWidgets[newTabIdx];
	ui->pageTitleLabel->setText(widget->widgetTitle());
	QSize pixmapSize = widget->widgetIcon().actualSize(QSize(64, 64));
	if (pixmapSize.isValid()) {
		ui->pageIconLabel->setPixmap(widget->widgetIcon().pixmap(pixmapSize));
	}
	else {
		ui->pageIconLabel->setPixmap(QPixmap());
	}
}

} /* end of namespace ui_AdvancedNS */

