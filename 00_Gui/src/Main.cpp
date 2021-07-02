#include <Windows.h>
#include <tchar.h>
#include "ui/resource.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	const TCHAR* title = TEXT("پیام");
	const TCHAR* text = TEXT("سلام، من یک پنجره هستم.");
	LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	MessageBox(NULL, text, title, MB_OKCANCEL | MB_ICONINFORMATION);
		

	return 0;
}