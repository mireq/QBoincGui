/*
 * =====================================================================
 *
 *       Filename:  CoreBoincPlugin.cpp
 *
 *        Version:  1.0
 *        Created:  17.01.2010 14:50:22
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "Engine.h"
#include "GuiIcon.h"
#include "InfoWidgets/SessionInfoWidget.h"
#include "CoreBoincPlugin.h"

namespace ui_AdvancedNS {

CoreBoincPlugin::CoreBoincPlugin(BoincTree *boincTree, QObject *parent):
	BoincPlugin(boincTree, parent)
{
}


CoreBoincPlugin::~CoreBoincPlugin()
{
}


void CoreBoincPlugin::registerSession(InfoBoinc::Session::IdType id)
{
	Session *session = Engine::getInstance().session(id);
	updateSessionState(session->state(), id);
	connect(session, SIGNAL(stateChanged(InfoBoinc::Session::State, InfoBoinc::Session::IdType)), SLOT(updateSessionState(InfoBoinc::Session::State, InfoBoinc::Session::IdType)));
}


void CoreBoincPlugin::unregisterSession(InfoBoinc::Session::IdType /*id*/)
{
}


QList<InfoWidget *> CoreBoincPlugin::createInfoWidgets(QTreeWidgetItem *item)
{
	QList<InfoWidget *> widgets;
	InfoBoinc::Session::IdType sessionId = 0;
	QTreeWidgetItem *sessionItem = item;
	while (sessionItem->parent() != 0) {
		sessionItem = sessionItem->parent();
	}
	sessionId = sessionItem->data(0, BoincTree::IdRole).toUInt();

	Session *session = Engine::getInstance().session(sessionId);
	switch (item->type()) {
		case HostType: break;
		default:
			widgets.append(new SessionInfoWidget(session));
	}
	return widgets;
}


void CoreBoincPlugin::updateSessionState(InfoBoinc::Session::State state, InfoBoinc::Session::IdType sessionId)
{
	if (state == InfoBoinc::Session::ConnectedState && !m_treeItems.contains(sessionId)) {
		QTreeWidgetItem *hostItem = new QTreeWidgetItem(HostType);
		hostItem->setText(0, tr("Host info"));
		hostItem->setIcon(0, GuiIcon("computer", "tree"));
	
		QList<QTreeWidgetItem *> items;
		items.append(hostItem);
	
		boincTree()->addTreeItems(sessionId, items);
	
		m_treeItems[sessionId] = items;
	}
	else if (m_treeItems.contains(sessionId)) {
		boincTree()->removeTreeItems(sessionId, m_treeItems[sessionId]);
		foreach (QTreeWidgetItem *item, m_treeItems[sessionId]) {
			delete item;
		}
		m_treeItems.remove(sessionId);
	}
}

} /* end of namespace ui_AdvancedNS */

