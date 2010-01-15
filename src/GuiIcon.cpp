/*
 * =====================================================================
 *
 *       Filename:  GuiIcon.cpp
 *
 *        Version:  1.0
 *        Created:  15.01.2010 20:50:45
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtCore/QFile>
#include "GuiIcon.h"

const char *GuiIcon::IconPath = ":/icons";
const char *GuiIcon::Suffix = ".png";
const int GuiIcon::Sizes[] = {8, 16, 22, 32, 48, 64, 128, 0};


GuiIcon::GuiIcon(const QString &name, const QString &category)
{
	for (int sizeIdx = 0; Sizes[sizeIdx] != 0; ++sizeIdx) {
		int size = Sizes[sizeIdx];
		QString fileName = QString("%1/%2x%2/%3/%4%5").arg(IconPath).arg(size).arg(category).arg(name).arg(Suffix);
		if (QFile::exists(fileName)) {
			addFile(fileName, QSize(size, size));
		}
	}
}

