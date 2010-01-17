/*
 * =====================================================================
 *
 *       Filename:  BoincTree.cpp
 *
 *        Version:  1.0
 *        Created:  14.01.2010 16:22:07
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include "Engine.h"
#include "GuiIcon.h"
#include "BoincTree.h"

using namespace InfoBoinc;
namespace ui_AdvancedNS {

BoincTree::BoincTree(QWidget *parent):
	QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	m_tree = new QTreeWidget;
	layout->addWidget(m_tree);

	m_tree->setHeaderHidden(true);
	m_tree->setSortingEnabled(true);
	m_tree->setAlternatingRowColors(true);
	m_tree->setIconSize(QSize(32, 32));

	connect(m_tree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), SLOT(changeTreeSelection(QTreeWidgetItem *)));
}


BoincTree::~BoincTree()
{
}


void BoincTree::addSession(Session::IdType id)
{
	Session *session = Engine::getInstance().session(id);
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setData(0, IdRole, id);
	item->setData(0, UserItemRole, QString("%1:%2").arg(session->host()).arg(session->port()));
	item->setData(0, Qt::DisplayRole, item->data(0, UserItemRole));
	m_tree->addTopLevelItem(item);
	m_tree->setItemExpanded(item, true);
	m_sessionItems[id] = item;
	changeSessionState(session->state(), session->id());
	connect(session, SIGNAL(stateChanged(Session::State, Session::IdType)), SLOT(changeSessionState(Session::State, Session::IdType)));
}


void BoincTree::removeSession(Session::IdType id)
{
	QTreeWidgetItem *item = m_sessionItems[id];
	m_tree->removeItemWidget(item, 0);
	m_sessionItems.remove(id);
	delete item;
}


void BoincTree::addTreeItems(Session::IdType id, QList<QTreeWidgetItem *> items)
{
	m_sessionItems[id]->addChildren(items);
}


void BoincTree::removeTreeItems(Session::IdType id, QList<QTreeWidgetItem *> items)
{
	QTreeWidgetItem *sessionItem = m_sessionItems[id];
	m_tree->setUpdatesEnabled(false);
	foreach (QTreeWidgetItem *item, items) {
		sessionItem->removeChild(item);
	}
	m_tree->setUpdatesEnabled(true);
}


void BoincTree::changeSessionState(Session::State state, Session::IdType id)
{
	QTreeWidgetItem *item = m_sessionItems[id];
	QString stateString;
	switch (state) {
		case Session::UnconnectedState:
			item->setIcon(0, GuiIcon("connect_no", "tree"));
			stateString = tr("Unconnected");
			break;
		case Session::DisconnectingState:
			item->setIcon(0, GuiIcon("connect_no", "tree"));
			stateString = tr("Disconnecting");
			break;
		case Session::ConnectingState:
			item->setIcon(0, GuiIcon("connect_creating", "tree"));
			stateString = tr("Connecting");
			break;
		case Session::ConnectedState:
			item->setIcon(0, GuiIcon("connect_established", "tree"));
			stateString = tr("Connected");
			break;
	}

	item->setData(0, Qt::DisplayRole, item->data(0, UserItemRole).toString() + " (" + stateString + ")");
}


void BoincTree::changeTreeSelection(QTreeWidgetItem *current)
{
	emit currentItemChanged(current);
}

} /* end of namespace ui_AdvancedNS */

