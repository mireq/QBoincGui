/*
 * =====================================================================
 *
 *       Filename:  main.cpp
 *
 *        Version:  1.0
 *        Created:  25.12.2009 14:10:49
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtGlobal>
#include <QtGui/QApplication>
#include "debug.h"
#include "src/Engine.h"

extern "C"
int main(int argc, char *argv[])
{
	qInstallMsgHandler(infoBoincMsgHandler);
	QApplication app(argc, argv);
	int ret = Engine::getInstance().initialize();
	if (ret != 0) {
		return ret;
	}
	return app.exec();
}

