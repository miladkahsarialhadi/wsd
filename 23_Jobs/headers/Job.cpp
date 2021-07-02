#include "Job.h"

Job::Job()
{
	HANDLE l_handle_current_process = GetCurrentProcess();
	IsProcessInJob(l_handle_current_process, NULL, &m_already_process_in_job);
}

BOOL Job::Check()
{
	if (m_already_process_in_job) {
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Job::RestrictedInformation()
{
	m_handle_job_object = CreateJobObject(NULL, L"Sandbox");

	m_limited_job_object_info.PriorityClass = IDLE_PRIORITY_CLASS;
	m_limited_job_object_info.PerJobUserTimeLimit.QuadPart = 10000;
	m_limited_job_object_info.ActiveProcessLimit = 2;
	m_limited_job_object_info.LimitFlags =	  JOB_OBJECT_LIMIT_PRIORITY_CLASS 
											| JOB_OBJECT_LIMIT_JOB_TIME 
											| JOB_OBJECT_LIMIT_ACTIVE_PROCESS;

	BOOL l_result_operation =	SetInformationJobObject(m_handle_job_object, 
														JobObjectBasicLimitInformation, 
														&m_limited_job_object_info, 
														sizeof(m_limited_job_object_info));

	if (l_result_operation)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL Job::RestrictedUi()
{
	m_limitied_job_object_ui.UIRestrictionsClass =    JOB_OBJECT_UILIMIT_NONE
													| JOB_OBJECT_UILIMIT_EXITWINDOWS
													| JOB_OBJECT_UILIMIT_HANDLES;

	BOOL l_result_operation =	SetInformationJobObject(m_handle_job_object,
														JobObjectBasicUIRestrictions, 
														&m_limitied_job_object_ui, 
														sizeof(m_limitied_job_object_ui));
	if (l_result_operation)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Job::GetJobInformation()
{
	// We can use QueryInformationJobObject to retrieve information about job's settings
	BOOL l_result_operation = QueryInformationJobObject(NULL, 
														JobObjectBasicUIRestrictions, 
														&m_get_information_job_object_ui, 
														sizeof(JOBOBJECT_BASIC_UI_RESTRICTIONS), 
														NULL);

	if (l_result_operation)
	{
		printf("Job Ui Restriction: %x\n", m_get_information_job_object_ui.UIRestrictionsClass);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Job::SpawnProcessInJob()
{
	// Spawn the process that is to be in the job.
	// Note: You must first spawn the process and then place the process in the job. 
	// This means that the process' thread must be initially
	// suspended so that it can't execute any code outside of the job's restrictions.

	TCHAR szCmdLine[8];
	_tcscpy_s(szCmdLine, _countof(szCmdLine), TEXT("CMD"));
	BOOL bResult = CreateProcess(NULL, 
		szCmdLine, 
		NULL, 
		NULL, 
		FALSE, 
		CREATE_SUSPENDED | CREATE_NEW_CONSOLE, 
		NULL, 
		NULL, 
		&m_startup_info, 
		&m_process_info);

	// Place the process in the job.
	// Note: If this process spawns any children, the children are automatically part of the same job.
	AssignProcessToJobObject(m_handle_job_object, m_process_info.hProcess);

	// Now we can allow the child process' thread to execute code.
	ResumeThread(m_process_info.hThread);

	return TRUE;
}

BOOL Job::CheckProcessInJob()
{
	// Check which is the process in a job or not
	BOOL l_process_in_job;
	IsProcessInJob(m_process_info.hProcess, m_handle_job_object, &l_process_in_job);
	printf("Process in a job: %d\n", l_process_in_job);
	return l_process_in_job;
}

VOID Job::DisplayInformation()
{
	QueryInformationJobObject(m_handle_job_object, 
							  JobObjectBasicAccountingInformation, 
							  &m_basic_job_object_info, 
							  sizeof(JOBOBJECT_BASIC_ACCOUNTING_INFORMATION), NULL);

	printf("Total Processes: %d, Active: %d, Terminated: %d.\n",
			m_basic_job_object_info.TotalProcesses,
			m_basic_job_object_info.ActiveProcesses,
			m_basic_job_object_info.TotalTerminatedProcesses);

	printf("User time all processes: %I64d\n", 
			m_basic_job_object_info.TotalUserTime.QuadPart / 1000000);

	// Terminate the primary thread of the process
	CloseHandle(m_process_info.hThread);

	FILETIME l_creation_time;
	FILETIME l_exit_time;
	FILETIME l_kernel_time;
	FILETIME l_username;
	TCHAR l_info[MAX_PATH];

	GetProcessTimes(m_process_info.hProcess, 
					&l_creation_time, 
					&l_exit_time, 
					&l_kernel_time, 
					&l_username);

	StringCchPrintf(l_info,
					_countof(l_info),
					TEXT("Kernel = %u | User = %u\n"), 
					l_kernel_time.dwLowDateTime / 10000,
					l_username.dwLowDateTime / 10000);

	MessageBox(GetActiveWindow(), 
			   l_info,
			   L"Restricted Process times", 
			   MB_ICONINFORMATION | MB_OK);
}

Job::~Job()
{
	CloseHandle(m_process_info.hProcess);
	CloseHandle(m_handle_job_object);
}