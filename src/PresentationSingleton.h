/*
 * =====================================================================
 *
 *       Filename:  PresentationSingleton.h
 *
 *        Version:  1.0
 *        Created:  20.01.2010 16:41:57
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef PRESENTATIONSINGLETON_H
#define PRESENTATIONSINGLETON_H

/**
 * \class PresentationSingleton
 */
class PresentationSingleton
{
public:
	~PresentationSingleton();
	static PresentationSingleton &getInstance();

	QString byteCountToString(double bytes);
	QString timeIntervalToString(uint seconds);
	QString doubleToString(double i, char f = 'f', int prec = 2);

private:
	PresentationSingleton();
	PresentationSingleton(const PresentationSingleton &) {};
	PresentationSingleton & operator= (const PresentationSingleton &) { return *this; };
}; /* -----  end of class PresentationSingleton  ----- */

#endif    /* ----- #ifndef PRESENTATIONSINGLETON_H  ----- */

