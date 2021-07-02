#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>

struct Info
{
	char m_name[20];
	char m_family[20];
};

void MessageLoop(char* arg_name, char* arg_family)
{
	printf("%s %s\n", arg_name, arg_family);
}

DWORD WINAPI ShowMessage(void* arg_param)
{
	Info* OInfo = (Info*)arg_param;

	while (TRUE)
	{
		MessageLoop(OInfo->m_name, OInfo->m_family);
	}

	return 0;
}

int main(int argc, TCHAR* argv[])
{
	Info* Object = new Info;
	strcpy(Object->m_name, "Milad");
	strcpy(Object->m_family, "Kahsari Alhadi");

	DWORD id_thread;
	HANDLE handle_thread = CreateThread(NULL, 0, ShowMessage, Object, 0, &id_thread);

	if (handle_thread != NULL)
	{
		WaitForSingleObject(handle_thread, INFINITE);
		CloseHandle(handle_thread);
	}

	return 0;
}