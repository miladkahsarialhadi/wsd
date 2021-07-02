#include "Common.h"

#include <windows.h>
#include <StrSafe.h>

#pragma comment (lib, "psapi.lib")  

HANDLE g_HandleIoCompletion;
HANDLE g_handleThreadCompletion;

int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

	// Check if we are not already associated with a job.
	// If this is the case, there is no way to switch to another job.
	BOOL check_job = FALSE;
	IsProcessInJob(GetCurrentProcess(), NULL, &check_job);
	if (check_job) {
		MessageBox(NULL, TEXT("Process already in a job"), TEXT("Message"), MB_ICONINFORMATION | MB_OK);
		MessageBox(NULL, TEXT("Process already in a job"), TEXT("Message"), MB_ICONINFORMATION | MB_OK);
		return -1;
	}

	// Create the completion port that receives job notifications
	g_HandleIoCompletion = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	
	// Create a thread that waits on the completion port
	g_handleThreadCompletion = chBEGINTHREADEX(NULL, 0, JobNotify, NULL, 0, NULL);

	// Create the job object
	g_job.Create(NULL, TEXT("JobLab"));
	g_job.SetEndOfJobInfo(JOB_OBJECT_POST_AT_END_OF_JOB);
	g_job.AssociateCompletionPort(g_hIOCP, COMPKEY_JOBOBJECT);

	return(0);
}
