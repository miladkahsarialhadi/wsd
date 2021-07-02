#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#define	ENV_VAR_STRING_COUNT	(sizeof(envVarStrings)/sizeof(char*))
#define	INFO_BUFFER_SIZE		1024

void PrintError(const char* arg_msg)
{
	DWORD dwErrorNumber;
	wchar_t wcSystemMessage[256];
	wchar_t* pwcPointerSystemMessage;

	dwErrorNumber = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		dwErrorNumber, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		wcSystemMessage, 
		256, 
		NULL);

	// Trim the end of the line and terminate it with a null
	pwcPointerSystemMessage = wcSystemMessage;

	while ((*pwcPointerSystemMessage > 31) || (*pwcPointerSystemMessage == 9))
		++pwcPointerSystemMessage;

	do {
		*pwcPointerSystemMessage-- = 0;
	} while ((pwcPointerSystemMessage >= wcSystemMessage) && ((*pwcPointerSystemMessage == '.') || (*pwcPointerSystemMessage < 33)));

	// Display the message
	printf("\n\t%s failed with error %d (%ws)", arg_msg, dwErrorNumber, wcSystemMessage);
}

int wmain(int argc, char* argv[]) {
	wchar_t wcInformationBuffer[INFO_BUFFER_SIZE];
	DWORD dwBufferCharCount = INFO_BUFFER_SIZE;

	if (!GetWindowsDirectory(wcInformationBuffer, INFO_BUFFER_SIZE))
		PrintError("GetWindowsDirectory");
	printf("\nWindows Directory:  %ws", wcInformationBuffer);

	GetSystemDirectory(wcInformationBuffer, INFO_BUFFER_SIZE);
	printf("\nSystem Directory:   %ws", wcInformationBuffer);

	GetComputerName(wcInformationBuffer, &dwBufferCharCount);
	printf("\nComputer Name:      %ws", wcInformationBuffer);

	GetUserName(wcInformationBuffer, &dwBufferCharCount);
	printf("\nUserName:           %ws\n", wcInformationBuffer);

	return 20;
}

