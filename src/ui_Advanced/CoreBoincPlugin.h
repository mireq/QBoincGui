/*
 * =====================================================================
 *
 *       Filename:  CoreBoincPlugin.h
 *
 *        Version:  1.0
 *        Created:  17.01.2010 14:50:28
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef COREBOINCPLUGIN_H
#define COREBOINCPLUGIN_H

#include <QtGui/QTreeWidgetItem>
#include "BoincPlugin.h"

namespace ui_AdvancedNS {

class InfoWidget;

/**
 * \class CoreBoincPlugin
 */
class CoreBoincPlugin: public BoincPlugin
{
Q_OBJECT
public:
	enum ItemType {
		HostType = QTreeWidgetItem::UserType
	};

	CoreBoincPlugin(BoincTree *boincTree, QObject *parent = 0);
	~CoreBoincPlugin();

	virtual void registerSession(InfoBoinc::Session::IdType id);
	virtual void unregisterSession(InfoBoinc::Session::IdType id);
	virtual InfoWidget *createInfoWidget(QTreeWidgetItem *item);

private slots:
	void updateSessionState(InfoBoinc::Session::State state, InfoBoinc::Session::IdType sessionId);

private:
	QMap<InfoBoinc::Session::IdType, QList<QTreeWidgetItem *> > m_treeItems;
}; /* -----  end of class CoreBoincPlugin  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef COREBOINCPLUGIN_H  ----- */

