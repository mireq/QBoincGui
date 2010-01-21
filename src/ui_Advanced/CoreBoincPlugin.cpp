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

#include "BoincTreeItems.h"
#include "Engine.h"
#include "GuiIcon.h"
#include "InfoWidgets/ProjectInfoWidget.h"
#include "InfoWidgets/SessionInfoWidget.h"
#include "InfoWidgets/SystemInfoWidget.h"
#include "InfoWidgets/TransfersInfoWidget.h"
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
	connect(session, SIGNAL(projectsAdded(QList<QString>, InfoBoinc::Session::IdType)), SLOT(addProjects(QList<QString>, InfoBoinc::Session::IdType)));
	connect(session, SIGNAL(projectsRemoved(QList<QString>, InfoBoinc::Session::IdType)), SLOT(removeProjects(QList<QString>, InfoBoinc::Session::IdType)));
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
	sessionId = sessionItem->data(0, BoincTreeItem::IdRole).toUInt();

	Session *session = Engine::getInstance().session(sessionId);
	switch (item->type()) {
		case SystemType:
			widgets.append(new SystemInfoWidget(session));
			break;
		case FiletransfersType:
			widgets.append(new TransfersInfoWidget(session));
			break;
		case ProjectType:
			widgets.append(new ProjectInfoWidget(session, item->data(0, BoincTreeItem::IdRole).toString()));
			break;
		default:
			widgets.append(new SessionInfoWidget(session));
	}
	return widgets;
}


void CoreBoincPlugin::updateSessionState(InfoBoinc::Session::State state, InfoBoinc::Session::IdType sessionId)
{
	InfoBoinc::Session *session = Engine::getInstance().session(sessionId);
	if (state == InfoBoinc::Session::ConnectedState && !m_treeItems.contains(sessionId)) {
		QTreeWidgetItem *hostItem = new BoincTreeItem(SystemType);
		hostItem->setText(0, tr("System"));
		hostItem->setIcon(0, GuiIcon("computer", "tree"));
		QTreeWidgetItem *fileTransfersItem = new BoincTreeItem(FiletransfersType);
		fileTransfersItem->setText(0, tr("File transfers"));
		fileTransfersItem->setIcon(0, GuiIcon("filetransfers", "tree"));
	
		QList<QTreeWidgetItem *> items;
		items << hostItem << fileTransfersItem;
	
		boincTree()->addTreeItems(sessionId, items);
		m_treeItems[sessionId] = items;

		const QList<QString> projectURLs = session->projects();
		addProjects(projectURLs, sessionId);
	}
	else if (m_treeItems.contains(sessionId)) {
		if (m_projects.contains(sessionId)) {
			removeProjects(m_projects[sessionId].keys(), sessionId);
			m_projects.remove(sessionId);
		}
		boincTree()->removeTreeItems(sessionId, m_treeItems[sessionId]);
		foreach (QTreeWidgetItem *item, m_treeItems[sessionId]) {
			delete item;
		}
		m_treeItems.remove(sessionId);
	}
}


void CoreBoincPlugin::addProjects(const QList<QString> &projectURLs, InfoBoinc::Session::IdType sessionId)
{
	QList<QTreeWidgetItem *> toAdd;
	InfoBoinc::Session *session = Engine::getInstance().session(sessionId);
	foreach (const QString &projectURL, projectURLs) {
		const ProjectInfo project = session->project(projectURL);
		QTreeWidgetItem *projectItem = new BoincTreeItem(ProjectType);
		projectItem->setText(0, project.projectName());
		projectItem->setIcon(0, GuiIcon("project", "tree"));
		projectItem->setData(0, BoincTreeItem::IdRole, project.masterURL());
		toAdd << projectItem;
		m_projects[sessionId][project.primaryKey()] = projectItem;
	}
	boincTree()->addTreeItems(sessionId, toAdd);
}


void CoreBoincPlugin::removeProjects(const QList<QString> &projectURLs, InfoBoinc::Session::IdType sessionId)
{
	QMap<InfoBoinc::Session::IdType, QMap<QString, QTreeWidgetItem *> >::iterator projectsSessionIterator = m_projects.find(sessionId);
	if (projectsSessionIterator == m_projects.end()) {
		return;
	}

	QList<QTreeWidgetItem *> toRemove;
	foreach (const QString &projectURL, projectURLs) {
		QMap<QString, QTreeWidgetItem *>::iterator projectIterator = projectsSessionIterator.value().find(projectURL);
		if (projectIterator == projectsSessionIterator.value().end()) {
			continue;
		}
		toRemove << projectIterator.value();
	}

	boincTree()->removeTreeItems(sessionId, toRemove);
	foreach (QTreeWidgetItem *item, toRemove) {
		delete item;
	}
}

} /* end of namespace ui_AdvancedNS */

