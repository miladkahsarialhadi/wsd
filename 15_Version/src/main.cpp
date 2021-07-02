#include <windows.h>
#include <stdio.h>
#include <VersionHelpers.h>


BOOL IsWindowsVista() {

	// Prepare the OSVERSIONINFOEX structure to indicate Windows Vista.
	OSVERSIONINFOEX osver = { 0 };

	osver.dwOSVersionInfoSize = sizeof(osver);
	osver.dwMajorVersion = 6;
	osver.dwMinorVersion = 0;
	osver.dwPlatformId = VER_PLATFORM_WIN32_NT;

	// Prepare the condition mask.

	// You MUST initialize this to 0.
	DWORDLONG dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_PLATFORMID, VER_EQUAL);

	// Perform the version test.
	return VerifyVersionInfo(&osver, VER_MAJORVERSION | VER_MINORVERSION | VER_PLATFORMID, dwlConditionMask);
}

BOOL IsWindownXpLater()
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	int op = VER_GREATER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osvi.dwMajorVersion = 5;
	osvi.dwMinorVersion = 1;
	osvi.wServicePackMajor = 2;
	osvi.wServicePackMinor = 0;

	// Initialize the condition mask.

	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, op);

	// Perform the test.

	return VerifyVersionInfo(
		&osvi,
		VER_MAJORVERSION | VER_MINORVERSION |
		VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
		dwlConditionMask);
}

BOOL IsWindowsServerIsRunning()
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	int op = VER_GREATER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osvi.dwMajorVersion = 5;
	osvi.dwMinorVersion = 0;
	osvi.wServicePackMajor = 0;
	osvi.wServicePackMinor = 0;
	osvi.wProductType = VER_NT_SERVER;

	// Initialize the condition mask.

	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);

	// Perform the test.

	return VerifyVersionInfo(
		&osvi,
		VER_MAJORVERSION | VER_MINORVERSION |
		VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR |
		VER_PRODUCT_TYPE,
		dwlConditionMask);
}

int main(int argc, char* argv[]) 
{
	if (IsWindowsServerIsRunning())
		printf("The system meets the requirements.\n");
	else
		printf("The system does not meet the requirements.\n");

	if (IsWindownXpLater())
		printf("Operating system is later version of XP.\n");
	else
		printf("Operating system is XP.\n");

	if (IsWindowsVista())
		printf("Operating system is Windows Vista.\n");
	else
		printf("Operating system isn't Windows Vista.\n");


	return 0;
}