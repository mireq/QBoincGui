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

#include "AddClientWizard.h"
#include "CoreBoincPlugin.h"
#include "Engine.h"
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
	//connect(ui->boincTree, SIGNAL(currentItemChanged(QTreeWidgetItem *)), SLOT(changeCurrentPage(QTreeWidgetItem *)));
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::addSession(Session::IdType id)
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
			QString directory = wizard.field("directory").toString();
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
}

} /* end of namespace ui_AdvancedNS */

