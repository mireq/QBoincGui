/*
 * =====================================================================
 *
 *       Filename:  ElidedTextLabel.cpp
 *
 *        Version:  1.0
 *        Created:  18.01.2010 13:21:24
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include "ElidedTextLabel.h"

ElidedTextLabel::ElidedTextLabel(QWidget *parent):
	QLabel(parent),
	m_mode(Qt::ElideRight)
{
}


ElidedTextLabel::~ElidedTextLabel()
{
}


QString ElidedTextLabel::text() const
{
	return m_text;
}


Qt::TextElideMode ElidedTextLabel::elideMode() const
{
	return m_mode;
}


QSize ElidedTextLabel::minimumSizeHint() const
{
	QSize size = QLabel::minimumSizeHint();
	if (m_mode != Qt::ElideNone) {
		size.setWidth(0);
	}
	return size;
}


void ElidedTextLabel::setText(const QString &text)
{
	m_text = text;
	updateTextLabel();
}


void ElidedTextLabel::setElideMode(Qt::TextElideMode mode)
{
	m_mode = mode;
	updateTextLabel();
}


void ElidedTextLabel::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
	updateTextLabel();
}


void ElidedTextLabel::updateTextLabel()
{
	// Neosekávame
	if (m_mode == Qt::ElideNone) {
		QLabel::setText(m_text);
		return;
	}

	QFontMetrics metrics(font());
	QLabel::setText(metrics.elidedText(m_text, m_mode, width()));
}



