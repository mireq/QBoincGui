/*
 * =====================================================================
 *
 *       Filename:  ui_Advanced.cpp
 *
 *        Version:  1.0
 *        Created:  06.01.2010 19:50:45
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "BoincTree.h"
#include "Engine.h"
#include "MainWindow.h"
#include "ui_Advanced.h"

using namespace ui_AdvancedNS;

ui_Advanced::ui_Advanced()
{
	m_mainWindow = new MainWindow;
	m_mainWindow->show();
}


ui_Advanced::~ui_Advanced()
{
	delete m_mainWindow;
}


void ui_Advanced::addSession(InfoBoinc::Session::IdType id)
{
	m_mainWindow->addSession(id);
}

