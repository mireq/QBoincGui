/*
 * =====================================================================================
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
 * =====================================================================================
 */

#include "AddClientWizard.h"
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
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_actionAddClient_triggered()
{
	AddClientWizard wizard;
	if (wizard.exec() == QDialog::Accepted) {
		QString host = wizard.field("host").toString();
		quint16 port = wizard.field("port").toUInt();
		QString pass = wizard.field("password").toString();
		quint32 id = Engine::getInstance().addSession(host, port, pass);
		QString directory;
		if (wizard.field("isLocal").toBool()) {
			QString directory = wizard.field("directory").toString();
			Engine::getInstance().session(id)->setDirectory(directory);
		}
	}
}

} /* end of namespace ui_AdvancedNS */

