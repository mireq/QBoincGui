/*
 * =====================================================================================
 *
 *       Filename:  AddClientWizard.cpp
 *
 *        Version:  1.0
 *        Created:  07.01.2010 16:00:16
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#include <QtCore/QVariant>
#include <QtGui/QRadioButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtNetwork/QHostAddress>
#include <limits>
#include "AddClientWizard.h"

#include <QDebug>

namespace ui_AdvancedNS {

AddClientWizard::AddClientWizard(QWidget *parent):
	QWizard(parent)
{
	setWindowTitle(tr("Add client"));
	addPage(new ClientTypeWizardPage);
	addPage(new ClientDataWizardPage);
}

AddClientWizard::~AddClientWizard()
{
}

ClientTypeWizardPage::ClientTypeWizardPage(QWidget *parent):
	QWizardPage(parent)
{
	setTitle(tr("Add client"));
	setSubTitle(tr("Select type of client"));
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	m_localRadio = new QRadioButton(tr("Local client"));
	m_remoteRadio = new QRadioButton(tr("Remote client"));
	layout->addWidget(m_localRadio);
	layout->addWidget(m_remoteRadio);

	m_localRadio->setChecked(true);
	registerField("isLocal", m_localRadio);
	registerField("isRemote", m_remoteRadio);
}

ClientTypeWizardPage::~ClientTypeWizardPage()
{
}

ClientDataWizardPage::ClientDataWizardPage(QWidget *parent):
	QWizardPage(parent),
	m_directoryVisible(true)
{
	setTitle(tr("Add client"));
	setSubTitle(tr("Enter connection preferences"));

	m_layout = new QFormLayout;
	setLayout(m_layout);

	m_directoryEdit = new QLineEdit;
	m_directorySelectButton = new QPushButton(tr("..."));
	connect(m_directoryEdit, SIGNAL(textChanged(QString)), SLOT(directoryChanged(QString)));
	connect(m_directorySelectButton, SIGNAL(clicked()), SLOT(selectDirectory()));
	m_directoryLabel = new QLabel(tr("B&oinc client directory"));
	m_directoryLabel->setBuddy(m_directoryEdit);
	m_directoryLayout = new QHBoxLayout;
	m_directoryLayout->addWidget(m_directoryEdit);
	m_directoryLayout->addWidget(m_directorySelectButton);
	registerField("directory", m_directoryEdit);

	m_layout->insertRow(0, m_directoryLabel, m_directoryLayout);

	m_hostEdit = new QLineEdit;
	m_layout->addRow(tr("&Host"), m_hostEdit);
	registerField("host", m_hostEdit);
	QHostAddress address(QHostAddress::LocalHost);
	m_hostEdit->setText(address.toString());

	m_portEdit = new QSpinBox;
	m_portEdit->setMinimum(std::numeric_limits<quint16>::min());
	m_portEdit->setMaximum(std::numeric_limits<quint16>::max());
	m_portEdit->setValue(31416);
	m_layout->addRow(tr("&Port"), m_portEdit);
	registerField("port", m_portEdit);

	m_passwordEdit = new QLineEdit;
	m_layout->addRow(tr("P&assword"), m_passwordEdit);
	registerField("password", m_passwordEdit);
}

ClientDataWizardPage::~ClientDataWizardPage()
{
	m_directoryLabel->deleteLater();
	m_directoryLayout->deleteLater();
}

void ClientDataWizardPage::initializePage()
{
	if (field("isLocal").toBool() && (!m_directoryVisible)) {
		m_layout->insertRow(0, m_directoryLabel, m_directoryLayout);
		m_directoryEdit->setHidden(false);
		m_directoryVisible = true;
	}
	else if (field("isRemote").toBool() && m_directoryVisible) {
		m_layout->removeWidget(m_directoryLabel);
		m_layout->removeItem(m_directoryLayout);
		m_directoryLabel->setParent(0);
		m_directoryLayout->setParent(0);
		m_directoryEdit->setHidden(true);
		m_directoryVisible = false;
	}
}

void ClientDataWizardPage::selectDirectory()
{
	QString directory = QFileDialog::getExistingDirectory(this, tr("Select boinc client directory"), m_directoryEdit->text());
	if (!directory.isNull()) {
		m_directoryEdit->setText(directory);
	}
}

void ClientDataWizardPage::directoryChanged(const QString &directory)
{
	const QString rpcFileName = directory + "/gui_rpc_auth.cfg";
	QFile rpcFile(rpcFileName);
	if (!rpcFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	if (rpcFile.isReadable()) {
		m_passwordEdit->setText(rpcFile.readAll());
	}
}


} /* end of namespace ui_AdvancedNS */

