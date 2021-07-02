#pragma once

#include <windows.h>
#include <malloc.h>

class CJob 
{
private:
	HANDLE m_handle_job;

public:
	CJob(HANDLE arg_handle_object = NULL);
	~CJob();

	operator HANDLE() const { return(m_handle_job); }

	// Functions to create/open a job object
	BOOL Create(PSECURITY_ATTRIBUTES arg_psa = NULL, PCTSTR arg_name = NULL);
	BOOL Open(PCTSTR arg_name, DWORD arg_desired_access, BOOL arg_inherit_handle = FALSE);

	// Functions that manipulate a job object
	BOOL AssignProcess(HANDLE arg_handle_process);
	BOOL Terminate(UINT arg_exit_code = 0);

	// Functions that set limits/restrictions on the job
	BOOL SetExtendedLimitInfo(PJOBOBJECT_EXTENDED_LIMIT_INFORMATION arg_ex_limit_info, BOOL arg_preserve_job_time = FALSE);
	BOOL SetBasicUIRestrictions(DWORD arg_limits);
	BOOL GrantUserHandleAccess(HANDLE arg_handle_object, BOOL arg_grant = TRUE);
	BOOL SetSecurityLimitInfo(PJOBOBJECT_SECURITY_LIMIT_INFORMATION arg_security_limit_info);

	// Functions that query job limits/restrictions
	BOOL QueryExtendedLimitInfo(PJOBOBJECT_EXTENDED_LIMIT_INFORMATION arg_ex_limit_info);
	BOOL QueryBasicUIRestrictions(PDWORD arg_restrictions);
	BOOL QuerySecurityLimitInfo(PJOBOBJECT_SECURITY_LIMIT_INFORMATION arg_ex_limit_info);

	// Functions that query job status information
	BOOL QueryBasicAccountingInfo(PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION arg_basic_io_accounting);
	BOOL QueryBasicProcessIdList(DWORD arg_max_process, PDWORD arg_process_id_list, PDWORD arg_processes_returned = NULL);

	// Functions that set/query job event notifications
	BOOL AssociateCompletionPort(HANDLE arg_handle_iocp, ULONG_PTR arg_compare_key);
	BOOL QueryAssociatedCompletionPort(PJOBOBJECT_ASSOCIATE_COMPLETION_PORT arg_associate_completion_port);
	BOOL SetEndOfJobInfo(DWORD arg_end_job_info = JOB_OBJECT_TERMINATE_AT_END_OF_JOB);
	BOOL QueryEndOfJobTimeInfo(PDWORD arg_end_job_time_info);
};


inline CJob::CJob(HANDLE arg_handle_job) 
{
	m_handle_job = arg_handle_job;
}


inline CJob::~CJob() 
{
	if (m_handle_job != NULL)
		CloseHandle(m_handle_job);
}

inline BOOL CJob::Create(PSECURITY_ATTRIBUTES arg_psa, PCTSTR arg_name) 
{
	m_handle_job = CreateJobObject(arg_psa, arg_name);
	return(m_handle_job != NULL);
}

inline BOOL CJob::Open(PCTSTR arg_name, DWORD arg_desired_access, BOOL arg_inherit_handle) 
{
	m_handle_job = OpenJobObject(arg_desired_access, arg_inherit_handle, arg_name);
	return(m_handle_job != NULL);
}

inline BOOL CJob::AssignProcess(HANDLE arg_handle_process) 
{
	return(AssignProcessToJobObject(m_handle_job, arg_handle_process));
}

inline BOOL CJob::AssociateCompletionPort(HANDLE arg_handle_iocp, ULONG_PTR arg_compare_key) 
{
	JOBOBJECT_ASSOCIATE_COMPLETION_PORT joacp = { (PVOID)arg_compare_key, arg_handle_iocp };
	return(SetInformationJobObject(m_handle_job, JobObjectAssociateCompletionPortInformation, &joacp, sizeof(joacp)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetExtendedLimitInfo(PJOBOBJECT_EXTENDED_LIMIT_INFORMATION arg_ex_limit_info, BOOL arg_preserve_job_time) {

	if (arg_preserve_job_time)
		arg_ex_limit_info->BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME;

	// If we are to preserve the job's time information, 
	// the JOB_OBJECT_LIMIT_JOB_TIME flag must not be on
	const DWORD fdwFlagTest = (JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME | JOB_OBJECT_LIMIT_JOB_TIME);

	if ((arg_ex_limit_info->BasicLimitInformation.LimitFlags & fdwFlagTest) == fdwFlagTest) {
		// These flags are mutually exclusive but both are on, error
		DebugBreak();
	}

	return(SetInformationJobObject(m_handle_job, JobObjectExtendedLimitInformation, arg_ex_limit_info, sizeof(*arg_ex_limit_info)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetBasicUIRestrictions(DWORD arg_limits) {

	JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir = { arg_limits };
	return(SetInformationJobObject(m_handle_job, JobObjectBasicUIRestrictions, &jobuir, sizeof(jobuir)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetEndOfJobInfo(DWORD arg_end_job_info) {

	JOBOBJECT_END_OF_JOB_TIME_INFORMATION object_end_time_info = { arg_end_job_info };
	object_end_time_info.EndOfJobTimeAction = arg_end_job_info;
	return(SetInformationJobObject(m_handle_job, JobObjectEndOfJobTimeInformation, &object_end_time_info, sizeof(object_end_time_info)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetSecurityLimitInfo(PJOBOBJECT_SECURITY_LIMIT_INFORMATION arg_security_limit_info)
{
	return(SetInformationJobObject(m_handle_job, JobObjectSecurityLimitInformation, arg_security_limit_info, sizeof(*arg_security_limit_info)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryAssociatedCompletionPort(PJOBOBJECT_ASSOCIATE_COMPLETION_PORT arg_associate_completion_port)
{

	return(QueryInformationJobObject(m_handle_job, JobObjectAssociateCompletionPortInformation, arg_associate_completion_port, sizeof(*arg_associate_completion_port), NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryBasicAccountingInfo(PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION arg_basic_io_accounting)
{

	return(QueryInformationJobObject(m_handle_job, JobObjectBasicAndIoAccountingInformation, arg_basic_io_accounting, sizeof(*arg_basic_io_accounting), NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryExtendedLimitInfo(PJOBOBJECT_EXTENDED_LIMIT_INFORMATION arg_ex_limit_info) {

	return(QueryInformationJobObject(m_handle_job, JobObjectExtendedLimitInformation, arg_ex_limit_info, sizeof(*arg_ex_limit_info), NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryBasicProcessIdList(DWORD arg_max_processes, PDWORD arg_process_id_list, PDWORD arg_processes_returned) 
{

	// Calculate the # of bytes necessary
	DWORD cb = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + (sizeof(DWORD) * (arg_max_processes - 1));

	// Allocate those bytes from the stack
	PJOBOBJECT_BASIC_PROCESS_ID_LIST object_basic_process_id_list = (PJOBOBJECT_BASIC_PROCESS_ID_LIST)_alloca(cb);

	// Were those bytes allocated OK? If so, keep going
	BOOL status = (object_basic_process_id_list != NULL);

	if (status) {
		object_basic_process_id_list->NumberOfProcessIdsInList = arg_max_processes;
		status = ::QueryInformationJobObject(m_handle_job, JobObjectBasicProcessIdList, object_basic_process_id_list, cb, NULL);

		if (status) {
			// We got the information, return it to the caller
			if (arg_processes_returned != NULL)
				*arg_processes_returned = object_basic_process_id_list->NumberOfProcessIdsInList;

			CopyMemory(arg_processes_returned, object_basic_process_id_list->ProcessIdList, sizeof(DWORD) * object_basic_process_id_list->NumberOfProcessIdsInList);
		}
	}
	return(status);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryBasicUIRestrictions(PDWORD pfdwRestrictions) 
{
	JOBOBJECT_BASIC_UI_RESTRICTIONS object_basic_ui_restriction;
	BOOL status = QueryInformationJobObject(m_handle_job, JobObjectBasicUIRestrictions, &object_basic_ui_restriction, sizeof(object_basic_ui_restriction), NULL);
	if (status)
		*pfdwRestrictions = object_basic_ui_restriction.UIRestrictionsClass;

	return(status);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryEndOfJobTimeInfo(PDWORD arg_end_job_time_info) 
{
	JOBOBJECT_END_OF_JOB_TIME_INFORMATION object_end_job_time_info;
	BOOL status = QueryInformationJobObject(m_handle_job, JobObjectBasicUIRestrictions, &object_end_job_time_info, sizeof(object_end_job_time_info), NULL);
	if (status)
		*arg_end_job_time_info = object_end_job_time_info.EndOfJobTimeAction;
	return(status);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QuerySecurityLimitInfo(PJOBOBJECT_SECURITY_LIMIT_INFORMATION arg_security_limit_info) 
{
	return(QueryInformationJobObject(m_handle_job, JobObjectSecurityLimitInformation, arg_security_limit_info, sizeof(*arg_security_limit_info), NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::Terminate(UINT arg_exit_code) {

	return(TerminateJobObject(m_handle_job, arg_exit_code));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::GrantUserHandleAccess(HANDLE arg_handle_user_object, BOOL arg_grant) {

	return(UserHandleGrantAccess(arg_handle_user_object, m_handle_job, arg_grant));
}
