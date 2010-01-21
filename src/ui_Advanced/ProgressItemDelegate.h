/*
 * =====================================================================
 *
 *       Filename:  ProgressItemDelegate.h
 *
 *        Version:  1.0
 *        Created:  21.01.2010 10:27:31
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef PROGRESSITEMDELEGATE_H
#define PROGRESSITEMDELEGATE_H

#include <QtGui/QStyledItemDelegate>

namespace ui_AdvancedNS {

/**
 * \class ProgressItemDelegate
 */
class ProgressItemDelegate: public QStyledItemDelegate
{
Q_OBJECT
public:
	ProgressItemDelegate(QObject *parent = 0);
	~ProgressItemDelegate();
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
}; /* -----  end of class ProgressItemDelegate  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef PROGRESSITEMDELEGATE_H  ----- */

