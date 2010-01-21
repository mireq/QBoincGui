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

class BoincTree: public QWidget
{
Q_OBJECT
public:
	BoincTree(QWidget *parent = 0);
	~BoincTree();
	void addSession(InfoBoinc::Session::IdType id);
	void removeSession(InfoBoinc::Session::IdType id);
	void addTreeItems(InfoBoinc::Session::IdType id, QList<QTreeWidgetItem *> items);
	void removeTreeItems(InfoBoinc::Session::IdType id, QList<QTreeWidgetItem *> items);

signals:
	void currentItemChanged(QTreeWidgetItem *item);

private slots:
	void changeSessionState(InfoBoinc::Session::State state, InfoBoinc::Session::IdType id);
	void changeTreeSelection(QTreeWidgetItem *current);

private:
	QTreeWidget *m_tree;
	QMap<InfoBoinc::Session::IdType, QTreeWidgetItem *> m_sessionItems;

}; /* -----  end of class BoincTree  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef BOINCTREE_H  ----- */

