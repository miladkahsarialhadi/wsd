#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <strsafe.h>

BOOL GetLastWriteTimeDirectory(HANDLE arg_h_file, LPSTR arg_lpsz_string, DWORD arg_dw_size)
{
	FILETIME ft_CreateTime, ft_AccessTime, ft_WriteTime;
	SYSTEMTIME st_UTC, st_Local;
	DWORD dw_Return;

	// Retrieve the file times for the file.
	if (!GetFileTime(arg_h_file, &ft_CreateTime, &ft_AccessTime, &ft_WriteTime))
	{
		return FALSE;
	}

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ft_WriteTime, &st_UTC);
	SystemTimeToTzSpecificLocalTime(NULL, &st_UTC, &st_Local);

	// Build a string showing the date and time.
	dw_Return = StringCchPrintfA(arg_lpsz_string, arg_dw_size, "%02d/%02d/%d  %02d:%02d", st_Local.wMonth, st_Local.wDay, st_Local.wYear, st_Local.wHour, st_Local.wMinute);

	if (S_OK == dw_Return)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool AttributeLastAccessDirectory(const char* arg_path)
{
	HANDLE handleFile;
	char bufferLastAccessTime[MAX_PATH];
	char pathDirectory[MAX_PATH];

	strcpy(pathDirectory, arg_path);

	handleFile = CreateFileA(pathDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (handleFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if (GetLastWriteTimeDirectory(handleFile, bufferLastAccessTime, MAX_PATH))
	{
		printf("\n\t\t");
		printf("%s", "Last Accessed: \t");
		printf("%s\n", bufferLastAccessTime);
		CloseHandle(handleFile);
		return true;
	}

	CloseHandle(handleFile);
	return false;
}


int main(int argc, char* argv[])
{
	AttributeLastAccessDirectory("\\\\Sevenx64\\C$\\Users\\Administrator\\Desktop\\Milad\\");

	return 0;
}