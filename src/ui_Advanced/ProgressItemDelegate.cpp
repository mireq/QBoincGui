/*
 * =====================================================================
 *
 *       Filename:  ProgressItemDelegate.cpp
 *
 *        Version:  1.0
 *        Created:  21.01.2010 10:27:24
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include "ProgressItemDelegate.h"

namespace ui_AdvancedNS {

ProgressItemDelegate::ProgressItemDelegate(QObject *parent):
	QStyledItemDelegate(parent)
{
}

ProgressItemDelegate::~ProgressItemDelegate()
{
}

void ProgressItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	opt.text = "";
	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, qobject_cast<QWidget *>(parent()));
	int progress = index.data(Qt::DisplayRole).toInt();
	QStyleOptionProgressBar progressBarOption;
	progressBarOption.rect = option.rect;
	progressBarOption.minimum = 0;
	progressBarOption.maximum = 100;
	progressBarOption.progress = progress;
	progressBarOption.textVisible = true;
	progressBarOption.text = QString::number(progress) + "%";
	QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, qobject_cast<QWidget *>(parent()));
	painter->restore();
}

} /* end of namespace ui_AdvancedNS */

