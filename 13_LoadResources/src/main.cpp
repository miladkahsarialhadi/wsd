#include <Windows.h>
#include <tchar.h>
#include "resource.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	if (LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)) == NULL) {
		const TCHAR* title = TEXT("پیام");
		const TCHAR* text = TEXT("سلام، آیکون برنامه تغییر نکرد.");
		MessageBox(NULL, text, title, MB_OKCANCEL | MB_ICONINFORMATION);
	}
	else
	{
		const TCHAR* title = TEXT("پیام");
		const TCHAR* text = TEXT("سلام، ماموریت با موفقیت انجام شد.");
		MessageBox(NULL, text, title, MB_OKCANCEL | MB_ICONINFORMATION);
	}

	return 0;
}