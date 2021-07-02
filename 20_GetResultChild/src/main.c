#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	TCHAR wcProgramName[] = TEXT("CMD");
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	DWORD dwExitCode;

	BOOL fSuccess = CreateProcess(
		NULL,
		wcProgramName,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);

	
	if (fSuccess) {
		// Close the thread handle as soon as it is no longer needed!
		CloseHandle(pi.hThread);

		// Suspend our execution until the child has terminated.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// The child process terminated; get its exit code.
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		printf("Exit Code: %d", dwExitCode);

		// Close the process handle as soon as it is no longer needed.
		CloseHandle(pi.hProcess);
	}

	return 0x0;
}