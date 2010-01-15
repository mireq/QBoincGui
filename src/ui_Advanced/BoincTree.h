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
#include <QtCore/QMap>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class QTreeWidget;

namespace ui_AdvancedNS {

using namespace InfoBoinc;

class BoincTree: public QWidget
{
Q_OBJECT
public:
	BoincTree(QWidget *parent = 0);
	~BoincTree();
	void addSession(Session::IdType id);
	void removeSession(Session::IdType id);

private slots:
	void changeSessionState(Session::State state, Session::IdType id);

private:
	enum ItemRole {
		PlubinRole = QTreeWidgetItem::UserType,
		TypeRole,
		IdRole,
		UserItemRole
	};

	QTreeWidget *m_tree;
	QMap<Session::IdType, QTreeWidgetItem *> m_sessions;
}; /* -----  end of class BoincTree  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef BOINCTREE_H  ----- */

