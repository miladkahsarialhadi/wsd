#include <Windows.h>
#include <iostream>
#include <locale.h>
#include <algorithm>

int main(int argc, const char* argv[])
{
	SetConsoleTitle(L"برنامه آزمایشی");
	HANDLE HandleConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	setlocale(LC_ALL, "persian");
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	CONSOLE_FONT_INFOEX ConsoleFontInfo;
	
	ConsoleFontInfo.cbSize = sizeof(ConsoleFontInfo);
	ConsoleFontInfo.FontFamily = 25;
	ConsoleFontInfo.FontWeight = 200;
	ConsoleFontInfo.nFont = 0;
	const wchar_t FontName[] = L"Arial";
	ConsoleFontInfo.dwFontSize = { 10, 20 };

	std::copy(FontName, FontName + (sizeof(FontName) / sizeof(wchar_t)), ConsoleFontInfo.FaceName);
	SetCurrentConsoleFontEx(HandleConsole, false, &ConsoleFontInfo);

	const wchar_t EnglishMessage[] = L"Welcome to Programming.";
	const wchar_t PersianMessage[] = L"به برنامه نویسی خوش آمدید.";

	std::wcout << EnglishMessage << std::endl;
	std::wcout << PersianMessage << std::endl;

	return 0;
}