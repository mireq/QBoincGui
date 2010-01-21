/*
 * =====================================================================
 *
 *       Filename:  BoincTreeItem.cpp
 *
 *        Version:  1.0
 *        Created:  21.01.2010 14:37:47
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "BoincTreeItems.h"

namespace ui_AdvancedNS {

BoincTreeItem::BoincTreeItem(int type):
	QTreeWidgetItem(type)
{
}


bool BoincTreeItem::operator<(const QTreeWidgetItem &other) const
{
	if (type() == other.type()) {
		return data(0, IdRole).toString() < other.data(0, IdRole).toString();
	}
	else {
		return type() < other.type();
	}
}

} /* end of namespace ui_AdvancedNS */

