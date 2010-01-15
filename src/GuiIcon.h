/*
 * =====================================================================
 *
 *       Filename:  GuiIcon.h
 *
 *        Version:  1.0
 *        Created:  15.01.2010 20:50:47
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef GUIICON_H
#define GUIICON_H

#include <QIcon>

/**
 * \class GuiIcon
 */
class GuiIcon: public QIcon
{
public:
	GuiIcon(const QString &name, const QString &category = QString());

private:
	static const char *IconPath;
	static const char *Suffix;
	static const int Sizes[];
}; /* -----  end of class GuiIcon  ----- */

#endif    /* ----- #ifndef GUIICON_H  ----- */

