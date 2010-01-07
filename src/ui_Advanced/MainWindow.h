/*
 * =====================================================================================
 *
 *       Filename:  MainWindow.h
 *
 *        Version:  1.0
 *        Created:  07.01.2010 10:43:06
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui {
	class MainWindow;
}

namespace ui_AdvancedNS {

class MainWindow: public QMainWindow
{
Q_OBJECT
public:
	/* ====================  LIFECYCLE     ==================== */
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	Ui::MainWindow *ui;

	/* ====================  DATA MEMBERS  ==================== */
}; /* -----  end of class MainWindow  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef MAINWINDOW_H  ----- */

