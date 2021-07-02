#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

void DumpEnvironmentStrings()
{
	PTSTR pEnvironmentBlock = GetEnvironmentStrings();
	TCHAR szName[MAX_PATH];
	TCHAR szValue[MAX_PATH];
	PTSTR pszCurrent = pEnvironmentBlock;
	HRESULT hr = S_OK;
	PCTSTR pszPos = NULL;
	DWORD dwCurrent = 0;

	while (pszCurrent != NULL) {
		if (*pszCurrent != TEXT('=')) {
			// Look for '=' separator.
			pszPos = _tcschr(pszCurrent, TEXT('='));

			// Point now to the first character of the value.
			pszPos++;

			// Copy the variable name without the' ='
			size_t cbNameLength = (size_t)pszPos - (size_t)pszCurrent - sizeof(TCHAR);
			hr = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
			if (FAILED(hr)) {
				break;
			}

			// Copy the variable value with the last NULL character and allow truncation because this is for UI only.
			hr = StringCchCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos) + 1);
			if (SUCCEEDED(hr)) {
				_tprintf(TEXT("[%u] %s=%s\r\n"), dwCurrent, szName, szValue);
			}
			// something wrong happened; check for truncation.
			else if (hr == STRSAFE_E_INSUFFICIENT_BUFFER) {
				_tprintf(TEXT("[%u] %s=%s...\r\n"), dwCurrent, szName, szValue);
			}
			// This should never occur.
			else {
				_tprintf(TEXT("[%u] %s=???\r\n"), dwCurrent, szName);
				break;
			}
		}
		else {
			_tprintf(TEXT("[%u] %s\r\n"), dwCurrent, pszCurrent);
		}

		// Next variable please.
		dwCurrent++;

		// Move to the end of the string.
		while (*pszCurrent != TEXT('\0'))
			pszCurrent++;
		pszCurrent++;

		// Check if it was not the last string.
		if (*pszCurrent == TEXT('\0'))
			break;
	};

	// Don't forget to free the memory.
	FreeEnvironmentStrings(pEnvironmentBlock);
}

void DumpEnvironmentVariableByName(LPCTSTR arg_psz_variable_name)
{
	PTSTR pszValue = NULL;
	DWORD dwResult = GetEnvironmentVariable(arg_psz_variable_name, pszValue, 0);
	if (dwResult != 0)
	{
		DWORD dwActualSize = dwResult * sizeof(TCHAR);
		pszValue = (PTSTR) malloc(dwActualSize);
		GetEnvironmentVariable(arg_psz_variable_name, pszValue, dwActualSize);
		_tprintf(TEXT("%s=%s\n"), arg_psz_variable_name, pszValue);
		free(pszValue);
	}
	else
	{
		_tprintf(TEXT("%s=Unkownable\n"), arg_psz_variable_name);
	}
}

void SetNewEnvironmentVariable(LPCTSTR arg_psz_name, LPCTSTR arg_psz_value)
{
	SetEnvironmentVariable(arg_psz_name, arg_psz_value);
}

void ShowCurrentDirectory()
{
	TCHAR pszCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pszCurrentDirectory);
	_tprintf("%s\n", pszCurrentDirectory);
}

void ChangeCurrentDirectory(LPCSTR arg_psz_dest_directory)
{
	SetCurrentDirectory(arg_psz_dest_directory);
}

int _tmain(int argc, const char* argv[])
{
	DumpEnvironmentStrings();

	return 0;
}