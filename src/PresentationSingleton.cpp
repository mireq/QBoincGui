/*
 * =====================================================================
 *
 *       Filename:  PresentationSingleton.cpp
 *
 *        Version:  1.0
 *        Created:  20.01.2010 16:41:53
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtCore/QLocale>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include "PresentationSingleton.h"

PresentationSingleton::PresentationSingleton()
{
}


PresentationSingleton::~PresentationSingleton()
{
}


PresentationSingleton &PresentationSingleton::getInstance()
{
	static PresentationSingleton instance;
	return instance;
}


QString PresentationSingleton::byteCountToString(double bytes)
{
	const double kbSize = 1024;
	const double mbSize = kbSize*1024;
	const double gbSize = mbSize*1024;
	const double tbSize = gbSize*1024;

	double calculatedBytes = 0;
	int precision = 0;
	QString sizeUnit;
	if (bytes <= kbSize) {
		calculatedBytes = bytes;
		sizeUnit = QObject::tr("Byte(s)", "", int(bytes));
	}
	else if (bytes <= mbSize) {
		calculatedBytes = bytes / kbSize;
		sizeUnit = QObject::tr("kB");
	}
	else if (bytes <= gbSize) {
		calculatedBytes = bytes / mbSize;
		sizeUnit = QObject::tr("MB");
	}
	else if (bytes <= tbSize) {
		calculatedBytes = bytes / gbSize;
		sizeUnit = QObject::tr("GB");
	}
	else {
		calculatedBytes = bytes / tbSize;
		sizeUnit = QObject::tr("TB");
	}

	if (bytes <= kbSize) {
		precision = 0;
	}
	else {
		if (calculatedBytes < 10) {
			precision = 2;
		}
		else if (calculatedBytes < 100) {
			precision = 1;
		}
	}

	return QLocale::system().toString(calculatedBytes, 'f', precision) + " " + sizeUnit;
}


QString PresentationSingleton::timeIntervalToString(uint seconds)
{
	static uint dayLength = 86400;
	QTime time(0, 0, 0);
	time = time.addSecs(seconds % uint(dayLength));
	uint days = seconds / dayLength;
	QString daysString;
	if (days != 0) {
		daysString = QObject::tr("%n day(s) ", "", days);
	}
	return daysString + time.toString(Qt::SystemLocaleLongDate);
}

