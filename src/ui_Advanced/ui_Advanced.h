/*
 * =====================================================================================
 *
 *       Filename:  ui_Advanced.h
 *
 *        Version:  1.0
 *        Created:  06.01.2010 19:50:48
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef UI_STANDARD_H
#define UI_STANDARD_H

#include "ui_Master.h"

namespace ui_AdvancedNS {
	class MainWindow;
} /* end of namespace ui_AdvancedNS */

class ui_Advanced: public ui_Master
{
public:
	ui_Advanced();
	~ui_Advanced();

private:
	ui_AdvancedNS::MainWindow *m_mainWindow;
}; /* -----  end of class ui_Advanced  ----- */

#endif    /* ----- #ifndef UI_STANDARD_H  ----- */

