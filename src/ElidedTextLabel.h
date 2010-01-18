/*
 * =====================================================================
 *
 *       Filename:  ElidedTextLabel.h
 *
 *        Version:  1.0
 *        Created:  18.01.2010 13:21:27
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef ELIDEDTEXTLABEL_H
#define ELIDEDTEXTLABEL_H

#include <QtGui/QLabel>

class ElidedTextLabel: public QLabel
{
Q_OBJECT

public:
	Q_PROPERTY(QString text READ text WRITE setText USER true)
	Q_PROPERTY(Qt::TextElideMode elideMode READ elideMode WRITE setElideMode)

	ElidedTextLabel(QWidget *parent = 0);
	~ElidedTextLabel();
	QString text() const;
	Qt::TextElideMode elideMode() const;
	virtual QSize minimumSizeHint() const;

public slots:
	void setText(const QString &text);
	void setElideMode(Qt::TextElideMode mode);

protected:
	virtual void resizeEvent(QResizeEvent *event);

private:
	void updateTextLabel();

	QString m_text;
	Qt::TextElideMode m_mode;
}; /* -----  end of class ElidedTextLabel  ----- */

#endif    /* ----- #ifndef ELIDEDTEXTLABEL_H  ----- */

