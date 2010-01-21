/*
 * =====================================================================
 *
 *       Filename:  FileTransferInfo.cpp
 *
 *        Version:  1.0
 *        Created:  20.01.2010 16:37:24
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#include <QtCore/QDateTime>
#include <QtXml/QDomNodeList>
#include "FileTransferInfo.h"

namespace InfoBoinc {

QMap<QString, int> FileTransferInfo::m_fileInfoTypes;
QMap<QString, int> FileTransferInfo::m_fileXferTypes;
QMap<QString, int> FileTransferInfo::m_persistentFileXferTypes;

FileTransferInfo::FileTransferInfo(const QDomElement &fileTransferNode)
{
	if (m_fileInfoTypes.count() == 0) {
		m_fileInfoTypes["project_url"] = XMLAttributeMap::StringAttribute;
		m_fileInfoTypes["project_name"] = XMLAttributeMap::StringAttribute;
		m_fileInfoTypes["name"] = XMLAttributeMap::StringAttribute;
		m_fileInfoTypes["nbytes"] = XMLAttributeMap::DoubleAttribute;
		m_fileInfoTypes["status"] = XMLAttributeMap::IntAttribute;
		m_fileInfoTypes["generated_locally"] = XMLAttributeMap::ExistsAttribute;
		m_fileInfoTypes["upload_when_present"] = XMLAttributeMap::ExistsAttribute;
		m_fileInfoTypes["persistent_file_xfer"] = XMLAttributeMap::ExistsAttribute;
		m_fileInfoTypes["file_xfer"] = XMLAttributeMap::ExistsAttribute;
	}
	if (m_fileXferTypes.count() == 0) {
		m_fileXferTypes["bytes_xferred"] = XMLAttributeMap::DoubleAttribute;
		m_fileXferTypes["file_offset"] = XMLAttributeMap::DoubleAttribute;
		m_fileXferTypes["xfer_speed"] = XMLAttributeMap::DoubleAttribute;
		m_fileXferTypes["url"] = XMLAttributeMap::StringAttribute;
	}
	if (m_persistentFileXferTypes.count() == 0) {
		m_persistentFileXferTypes["num_retries"] = XMLAttributeMap::IntAttribute;
		m_persistentFileXferTypes["first_request_time"] = XMLAttributeMap::TimestampAttribute;
		m_persistentFileXferTypes["next_request_time"] = XMLAttributeMap::TimestampAttribute;
		m_persistentFileXferTypes["time_so_far"] = XMLAttributeMap::DoubleAttribute;
		m_persistentFileXferTypes["last_bytes_xferred"] = XMLAttributeMap::DoubleAttribute;
	}
	m_attributes.parseAttributes(fileTransferNode, m_fileInfoTypes);
	QDomNodeList persistentXferNodes = fileTransferNode.elementsByTagName("persistent_file_xfer");
	if (persistentXferNodes.count() > 0) {
		m_attributes.parseAttributes(persistentXferNodes.at(0).toElement(), m_persistentFileXferTypes);
	}
	QDomNodeList xferNodes = fileTransferNode.elementsByTagName("file_xfer");
	if (xferNodes.count() > 0) {
		m_attributes.parseAttributes(xferNodes.at(0).toElement(), m_fileXferTypes);
	}
}

FileTransferInfo::~FileTransferInfo()
{
}

QPair<QString, QString> FileTransferInfo::primaryKey() const
{
	return QPair<QString, QString>(projectUrl(), projectName());
}

QString FileTransferInfo::projectUrl() const
{
	return m_attributes.attribute("project_url").toString();
}

QString FileTransferInfo::projectName() const
{
	return m_attributes.attribute("project_name").toString();
}

QString FileTransferInfo::name() const
{
	return m_attributes.attribute("name").toString();
}

double FileTransferInfo::nBytes() const
{
	return m_attributes.attribute("nbytes").toDouble();
}

int FileTransferInfo::status() const
{
	return m_attributes.attribute("status").toInt();
}

bool FileTransferInfo::generatedLocally() const
{
	return m_attributes.attribute("generated_locally").toBool();
}

bool FileTransferInfo::uploadWhenPresent() const
{
	return m_attributes.attribute("upload_when_present").toBool();
}

bool FileTransferInfo::hasPersistentFileXfer() const
{
	return m_attributes.attribute("persistent_file_xfer").toBool();
}

bool FileTransferInfo::isXferActive() const
{
	return m_attributes.attribute("file_xfer").toBool();
}

double FileTransferInfo::bytesXferred() const
{
	return m_attributes.attribute("bytex_xferred").toDouble();
}

double FileTransferInfo::fileOffset() const
{
	return m_attributes.attribute("file_offset").toDouble();
}

double FileTransferInfo::xferSpeed() const
{
	return m_attributes.attribute("xfer_speed").toDouble();
}

QString FileTransferInfo::url() const
{
	return m_attributes.attribute("url").toString();
}

int FileTransferInfo::numRetries() const
{
	return m_attributes.attribute("num_retries").toInt();
}

QDateTime FileTransferInfo::firstRequestTime() const
{
	return m_attributes.attribute("first_request_time").toDateTime();
}

QDateTime FileTransferInfo::nextRequestTime() const
{
	return m_attributes.attribute("next_request_time").toDateTime();
}

double FileTransferInfo::timeElapsed() const
{
	return m_attributes.attribute("time_so_far").toDouble();
}

double FileTransferInfo::lastBytesXferred() const
{
	return m_attributes.attribute("last_bytes_xferred").toDouble();
}

bool operator==(const FileTransferInfo &lhs, const FileTransferInfo &rhs)
{
	return lhs.m_attributes == rhs.m_attributes;
}

bool operator!=(const FileTransferInfo &lhs, const FileTransferInfo &rhs)
{
	return lhs.m_attributes != rhs.m_attributes;
}

} /* end of namespace InfoBoinc */

