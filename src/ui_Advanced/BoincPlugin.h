/*
 * =====================================================================
 *
 *       Filename:  BoincPlugin.h
 *
 *        Version:  1.0
 *        Created:  17.01.2010 13:39:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef BOINCPLUGIN_H
#define BOINCPLUGIN_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include "BoincTree.h"
#include "Session.h"

namespace ui_AdvancedNS {

class InfoWidget;
using namespace InfoBoinc;
/**
 * \class BoincPlugin
 */
class BoincPlugin: public QObject
{
Q_OBJECT
public:
	BoincPlugin(BoincTree *boincTree, QObject *parent = 0): QObject(parent), m_boincTree(boincTree) {};
	
	virtual void registerSession(Session::IdType id) = 0;
	virtual void unregisterSession(Session::IdType id) = 0;
	virtual QList<InfoWidget *> createInfoWidgets(QTreeWidgetItem *item) = 0;

protected:
	BoincTree *boincTree() const { return m_boincTree; };

private:
	BoincTree *m_boincTree;
}; /* -----  end of class BoincPlugin  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef BOINCPLUGIN_H  ----- */

