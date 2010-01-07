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

} /* end of namespace ui_AdvancedNS */

