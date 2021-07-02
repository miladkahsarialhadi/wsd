#include "Job.h"

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{
		std::cout << "CreateProcess failed: " << GetLastError() << std::endl;
		return -1;
	}

	HANDLE handle_job_object = CreateJobObject(NULL, L"MiladSandbox");

	JOBOBJECT_BASIC_UI_RESTRICTIONS limitied_job_object_ui = { 0 };
	limitied_job_object_ui.UIRestrictionsClass = JOB_OBJECT_UILIMIT_EXITWINDOWS;

	BOOL result_operation = SetInformationJobObject(handle_job_object,
													JobObjectBasicUIRestrictions,
													&limitied_job_object_ui,
													sizeof(limitied_job_object_ui));

	AssignProcessToJobObject(handle_job_object, pi.hProcess);

	if (result_operation)
	{
		std::cout << "Job Object Initialized." << std::endl;
	}
	else
	{
		std::cout << "Job Object Didn't Initialized." << std::endl;
	}

	ResumeThread(pi.hThread);

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(handle_job_object);
	
	system("PAUSE");
	return 0;
}