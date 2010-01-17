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

#include "GuiIcon.h"
#include "CoreBoincPlugin.h"

namespace ui_AdvancedNS {

CoreBoincPlugin::CoreBoincPlugin(BoincTree *boincTree, QObject *parent):
	BoincPlugin(boincTree, parent)
{
}


CoreBoincPlugin::~CoreBoincPlugin()
{
}


void CoreBoincPlugin::registerSession(Session::IdType id)
{
	QTreeWidgetItem *hostItem = new QTreeWidgetItem(HostType);
	hostItem->setText(0, tr("Host info"));
	hostItem->setIcon(0, GuiIcon("computer", "tree"));

	QList<QTreeWidgetItem *> items;
	items.append(hostItem);

	boincTree()->addTreeItems(id, items);
}


void CoreBoincPlugin::unregisterSession(Session::IdType id)
{
}


InfoWidget *CoreBoincPlugin::createInfoWidget(QTreeWidgetItem *item)
{
	return 0;
}

} /* end of namespace ui_AdvancedNS */

