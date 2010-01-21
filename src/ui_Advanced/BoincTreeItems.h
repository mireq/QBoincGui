/*
 * =====================================================================
 *
 *       Filename:  BoincTreeItem.h
 *
 *        Version:  1.0
 *        Created:  21.01.2010 14:37:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef BOINCTREEITEM_H
#define BOINCTREEITEM_H

#include <QtGui/QTreeWidgetItem>

namespace ui_AdvancedNS {

/**
 * \class BoincTreeItem
 */
class BoincTreeItem: public QTreeWidgetItem
{
public:
	enum ItemRole {
		PluginRole = Qt::UserRole,
		IdRole,
		UserItemRole
	};

	BoincTreeItem(int type);
	bool operator<(const QTreeWidgetItem &other) const;
}; /* -----  end of class BoincTreeItem  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef BOINCTREEITEM_H  ----- */

