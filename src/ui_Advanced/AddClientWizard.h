/*
 * =====================================================================
 *
 *       Filename:  AddClientWizard.h
 *
 *        Version:  1.0
 *        Created:  07.01.2010 16:00:20
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef ADDCLIENTWIZARD_H
#define ADDCLIENTWIZARD_H

#include <QtGui/QWizard>

class QFormLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QSpinBox;

namespace ui_AdvancedNS {

class AddClientWizard: public QWizard
{
Q_OBJECT
public:
	AddClientWizard(QWidget *parent = 0);
	~AddClientWizard();
}; /* -----  end of class AddClientWizard  ----- */

class ClientTypeWizardPage: public QWizardPage
{
Q_OBJECT
public:
	ClientTypeWizardPage(QWidget *parent = 0);
	~ClientTypeWizardPage();

private:
	QRadioButton *m_localRadio;
	QRadioButton *m_remoteRadio;
}; /* -----  end of class ClientTypeWizardPage  ----- */

class ClientDataWizardPage: public QWizardPage
{
Q_OBJECT
public:
	ClientDataWizardPage(QWidget *parent = 0);
	~ClientDataWizardPage();
	virtual void initializePage();

private slots:
	void selectDirectory();
	void directoryChanged(const QString &directory);

private:
	QFormLayout *m_layout;
	QHBoxLayout *m_directoryLayout;
	QLabel *m_directoryLabel;
	QLineEdit *m_directoryEdit;
	QPushButton *m_directorySelectButton;
	QLineEdit *m_hostEdit;
	QSpinBox *m_portEdit;
	QLineEdit *m_passwordEdit;
	bool m_directoryVisible;
}; /* -----  end of class ClientDataWizardPage  ----- */

} /* end of namespace ui_AdvancedNS */

#endif    /* ----- #ifndef ADDCLIENTWIZARD_H  ----- */

