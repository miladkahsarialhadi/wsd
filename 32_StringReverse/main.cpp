#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>

BOOL StringReverseW(PWSTR arg_wide_char_string, DWORD char_length)
{
	PWSTR end_string = arg_wide_char_string + wcsnlen_s(arg_wide_char_string, char_length) - 1;
	wchar_t wchar_point;
	while (arg_wide_char_string < end_string) {
		wchar_point = *arg_wide_char_string;
		*arg_wide_char_string = *end_string;
		*end_string = wchar_point;
		arg_wide_char_string++;
		end_string--;
	}
	return(TRUE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	CHAR	ch[10] = "abcd";
	TCHAR	aa[10] = L"hello";
	TCHAR	bb[10] = L"world";
	CHAR	buffer_one[5], buffer_two[40];
	HRESULT	hr;
	LCID	lcid;
	LPCVOID	v;
	CHAR	wc[100];
	HLOCAL	hlc;
	TCHAR	rvws[20] = L"??,??";
	TCHAR	rvws2[20] = L"?? ,??";

	lcid = GetThreadLocale();
	hr = CompareString(lcid, NORM_IGNORECASE, aa, _countof(aa), bb, _countof(bb));
	if (hr == CSTR_EQUAL) {
		_tprintf(TEXT("equal %d\n"), lcid);
	}
	else if (hr == CSTR_GREATER_THAN) {
		_tprintf(TEXT("string1 greater than 2\n"));
	}
	else if (hr == CSTR_LESS_THAN) {
		_tprintf(TEXT("string1 less than 2\n"));
	}

	hr = WideCharToMultiByte(0, 0, bb, _countof(bb), buffer_one, _countof(buffer_one), NULL, FALSE);
	if (hr) {
		_tprintf(TEXT("buf = %s\n"), buffer_one);
	}
	else {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			GetLastError(), lcid, (PWSTR)&hlc, 0, NULL);

		_tprintf(TEXT("error %ws\n"), hlc);
		LocalFree(hlc);
	}

	if (IsTextUnicode(ch, _countof(ch), NULL)) {
		_tprintf(TEXT("Unicode\n"));
	}
	else {
		_tprintf(TEXT("Not unicode\n"));
	}

	StringReverseW(rvws, _countof(rvws));
	hr = CompareString(lcid, NORM_IGNORECASE, rvws, _countof(rvws), rvws2, _countof(rvws2));
	if (hr == CSTR_EQUAL) {
		_tprintf(TEXT("equal\n"));
	}
	else if (hr == CSTR_GREATER_THAN) {
		_tprintf(TEXT("string1 greater than 2\n"));
	}
	else if (hr == CSTR_LESS_THAN) {
		_tprintf(TEXT("string1 less than 2\n"));
	}

	return 0;
}