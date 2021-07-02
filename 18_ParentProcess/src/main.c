#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, const char* argv[])
{
	SECURITY_ATTRIBUTES process_sa;
	process_sa.nLength = sizeof(process_sa);
	process_sa.bInheritHandle = TRUE;
	process_sa.lpSecurityDescriptor = 0;

	SECURITY_ATTRIBUTES thread_sa;
	thread_sa.nLength = sizeof(thread_sa);
	thread_sa.bInheritHandle = TRUE;
	thread_sa.lpSecurityDescriptor = NULL;

	HANDLE hFileCreated = CreateFile("e:\\Sample.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, &process_sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFileCreated == INVALID_HANDLE_VALUE)
	{
		printf("File doesn't created.");
		return 1;
	}
	
	char c = '1';
	DWORD w;
	WriteFile(hFileCreated, &c, 1, &w, 0);

	STARTUPINFO sinfo;
	ZeroMemory(&sinfo, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);

	PROCESS_INFORMATION pinfo;
	ZeroMemory(&pinfo, sizeof(pinfo));

	if (CreateProcess(0, "ChildProcess.exe", &process_sa, &thread_sa, TRUE, 0, 0, 0, &sinfo, &pinfo))
		printf("done.");
	else
		printf("failed.");

	CloseHandle(pinfo.hThread);
	CloseHandle(pinfo.hProcess);
	

	return 0;
}
