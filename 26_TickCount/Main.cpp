#include <Windows.h>
#include <stdio.h>

namespace Tools
{
	const wchar_t* GenerateRandomPos(const wchar_t* arg_input);
}

const wchar_t* Tools::GenerateRandomPos(const wchar_t* arg_input)
{
	int increment = GetTickCount64() & lstrlenW(arg_input);
	return arg_input + increment;
}

int main(int argc, const char* argv[])
{
	const wchar_t* string_info = L"Milad Kahsari Alhadi";
	wprintf(L"%s\n", Tools::GenerateRandomPos(string_info));

	return 0;
}