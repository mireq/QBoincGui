/*
 * =====================================================================
 *
 *       Filename:  InfoWidget.h
 *
 *        Version:  1.0
 *        Created:  17.01.2010 17:13:37
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QtGui/QIcon>
#include <QtGui/QWidget>

namespace ui_AdvancedNS {

class InfoWidget: public QWidget
{
Q_OBJECT
public:
	InfoWidget(QWidget *parent = 0);
	~InfoWidget();

	virtual QString widgetTitle() const;
	virtual QIcon widgetIcon() const;
}; /* -----  end of class InfoWidget  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef INFOWIDGET_H  ----- */

