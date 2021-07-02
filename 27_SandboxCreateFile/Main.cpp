#include <windows.h>
#include <iostream>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	std::cout << "Process is running" << std::endl;
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPWSTR szCmdline = _tcsdup(TEXT("notepad.exe"));
	// ChildProcess 
	if (!CreateProcessW(NULL, szCmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		std::cout << "CreateProcess failed: " << GetLastError() << std::endl;
		return -1;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}