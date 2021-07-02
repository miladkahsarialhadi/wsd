#include <windows.h>
#include <iostream>

int main(int argc, const char* argv[])
{
	SECURITY_ATTRIBUTES saMutexAttribute;
	
	saMutexAttribute.nLength = sizeof(saMutexAttribute);
	saMutexAttribute.lpSecurityDescriptor = NULL;
	saMutexAttribute.bInheritHandle = TRUE;

	HANDLE hMutex = CreateMutex(&saMutexAttribute, FALSE, NULL);
	SetHandleInformation(hMutex, HANDLE_FLAG_PROTECT_FROM_CLOSE, HANDLE_FLAG_PROTECT_FROM_CLOSE);
	CloseHandle(hMutex);

	return 0;
}