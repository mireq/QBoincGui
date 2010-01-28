/*
 * =====================================================================
 *
 *       Filename:  ResultInfo.h
 *
 *        Version:  1.0
 *        Created:  22.01.2010 07:03:19
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================
 */

#ifndef RESULTINFO_H
#define RESULTINFO_H

#include "WorkunitInfo.h"
#include "XMLAttributeMap.h"
class QDomElement;

namespace InfoBoinc {

/**
 * \class ResultInfo
 */
class ResultInfo
{
public:
	enum State {
		NewState,
		DownloadingState,
		DownloadedState,
		ComputeErrorState,
		UploadingState,
		UploadedState,
		AbortedState
	};

	enum TaskState {
		UninitializedTaskState,
		ExecutingTaskState,
		ExitedTaTaskState,
		WasSignaledTaskState,
		ExitUnknownTaskState,
		AbortPendingTaskState,
		AbortedTaskState,
		CouldntStartTaskState,
		QuitPendingTaskState,
		SuspendedTaskState
	};

	/* ====================  LIFECYCLE     ==================== */
	ResultInfo();
	ResultInfo(const QDomElement &resultNode);
	~ResultInfo();

	/* ====================  MUTATORS      ==================== */
	void bindWorkunit(const WorkunitInfo &workunit);

	/* ====================  ACCESSORS     ==================== */
	bool isNull();
	const WorkunitInfo &workunit() const;
	QString primaryKey() const;
	QString name() const;
	QString wuName() const;
	int state() const;
	double fractionDone() const;
	int activeTaskState() const;

	/* ====================  OPERATORS     ==================== */
	friend bool operator==(const ResultInfo &lhs, const ResultInfo &rhs);
	friend bool operator!=(const ResultInfo &lhs, const ResultInfo &rhs);

private:
	static QMap<QString, int> m_resultAttributeTypes;
	static QMap<QString, int> m_activeTaskAttributeTypes;
	XMLAttributeMap m_attributes;
	WorkunitInfo m_workunit;
}; /* -----  end of class ResultInfo  ----- */

bool operator==(const ResultInfo &lhs, const ResultInfo &rhs);
bool operator!=(const ResultInfo &lhs, const ResultInfo &rhs);

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef RESULTINFO_H  ----- */

