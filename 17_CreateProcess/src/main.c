#include <Windows.h>

int main(int argc, char* argv[])
{
	TCHAR wcProgramName[] = TEXT("CMD");
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	CreateProcess(
		NULL,
		wcProgramName,
		NULL,
		NULL,
		FALSE,
		ABOVE_NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&si,
		&pi
		);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0x0;
}