/*
 * =====================================================================
 *
 *       Filename:  FileTransferInfo.h
 *
 *        Version:  1.0
 *        Created:  20.01.2010 16:37:39
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef FILETRANSFERINFO_H
#define FILETRANSFERINFO_H

#include <QtCore/QPair>
#include <QtCore/QString>
#include "XMLAttributeMap.h"
class QDomElement;

namespace InfoBoinc {

/**
 * \class FileTransferInfo
 */
class FileTransferInfo
{
public:
	enum Status {
		FilePresentStatus = 0,
		FileNotPresentStatus = 1,
		ErrorUploadStatus = -115,
		ErrorDownloadStatus = -114
	};

	/* ====================  LIFECYCLE     ==================== */
	FileTransferInfo(const QDomElement &fileTransferNode);
	~FileTransferInfo();

	/* ====================  ACCESSORS     ==================== */
	QPair<QString, QString> primaryKey() const;
	QString projectUrl() const;
	QString projectName() const;
	QString name() const;
	double nBytes() const;
	int status() const;
	bool generatedLocally() const;
	bool uploadWhenPresent() const;
	bool hasPersistentFileXfer() const;
	bool isXferActive() const;
	double bytesXferred() const;
	double fileOffset() const;
	double xferSpeed() const;
	QString url() const;
	int numRetries() const;
	QDateTime firstRequestTime() const;
	QDateTime nextRequestTime() const;
	double timeElapsed() const;
	double lastBytesXferred() const;

	/* ====================  OPERATORS     ==================== */
	friend bool operator==(const FileTransferInfo &lhs, const FileTransferInfo &rhs);
	friend bool operator!=(const FileTransferInfo &lhs, const FileTransferInfo &rhs);

private:
	static QMap<QString, int> m_fileInfoTypes;
	static QMap<QString, int> m_fileXferTypes;
	static QMap<QString, int> m_persistentFileXferTypes;
	XMLAttributeMap m_attributes;
}; /* -----  end of class FileTransferInfo  ----- */

bool operator==(const FileTransferInfo &lhs, const FileTransferInfo &rhs);
bool operator!=(const FileTransferInfo &lhs, const FileTransferInfo &rhs);

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef FILETRANSFERINFO_H  ----- */

