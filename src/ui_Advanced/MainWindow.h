/*
 * =====================================================================
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
 * =====================================================================
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "Session.h"

namespace Ui {
	class MainWindow;
}

namespace ui_AdvancedNS {

class BoincTree;
class CoreBoincPlugin;
class InfoWidget;
using namespace InfoBoinc;

class MainWindow: public QMainWindow
{
Q_OBJECT
public:
	/* ====================  LIFECYCLE     ==================== */
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void addSession(Session::IdType id);

private slots:
	void on_actionAddClient_triggered();
	void on_boincTree_currentItemChanged(QTreeWidgetItem *current);

private:
	Ui::MainWindow *ui;

	void setInfoWidget(InfoWidget *widget);

	/* ====================  DATA MEMBERS  ==================== */
	CoreBoincPlugin *m_coreBoincPlugin;
}; /* -----  end of class MainWindow  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef MAINWINDOW_H  ----- */

