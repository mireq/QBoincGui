/*
 * =====================================================================
 *
 *       Filename:  BoincTree.h
 *
 *        Version:  1.0
 *        Created:  14.01.2010 16:22:12
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef BOINCTREE_H
#define BOINCTREE_H

#include "Session.h"
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

namespace ui_AdvancedNS {

using namespace InfoBoinc;

class BoincTree: public QWidget
{
Q_OBJECT
public:
	enum ItemRole {
		PluginRole = Qt::UserRole,
		IdRole,
		UserItemRole
	};

	BoincTree(QWidget *parent = 0);
	~BoincTree();
	void addSession(Session::IdType id);
	void removeSession(Session::IdType id);
	void addTreeItems(Session::IdType id, QList<QTreeWidgetItem *> items);
	void removeTreeItems(Session::IdType id, QList<QTreeWidgetItem *> items);

signals:
	void currentItemChanged(QTreeWidgetItem *item);

private slots:
	void changeSessionState(Session::State state, Session::IdType id);
	void changeTreeSelection(QTreeWidgetItem *current);

private:
	QTreeWidget *m_tree;
	QMap<Session::IdType, QTreeWidgetItem *> m_sessionItems;

}; /* -----  end of class BoincTree  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef BOINCTREE_H  ----- */

