#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	HKEY hKey;
	const char* czStartName = "29_Registry";
	const char* czExePath = "C:\\Users\\mkahs\\Desktop\\29_Registry.exe";

	LONG lnRes = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);
	if (ERROR_SUCCESS == lnRes)
	{
		lnRes = RegSetValueEx(hKey, czStartName, 0, REG_SZ, (unsigned char*)czExePath, strlen(czExePath));
	}

	printf("IT WORKS\n");
	system("PAUSE");
	RegCloseKey(hKey);

	return 0;
}