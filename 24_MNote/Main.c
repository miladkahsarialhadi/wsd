#include <Windows.h>
#include <tchar.h>
#include "resource.h"

typedef struct  
{
	wchar_t filename[MAX_PATH];
}DOCUMENT_STATE;

typedef struct
{
	DOCUMENT_STATE document;
	HWND handle_window;
	HWND handle_window_edit;
	HINSTANCE handle_instance;
	HICON handle_icon;
	HFONT handle_font;
}APP_STATE;

APP_STATE app;
const wchar_t* MNoteWindowClass = L"MNote";

BOOL MNoteOpenFile(const wchar_t* arg_filename)
{
	BOOL success = FALSE;
	BYTE* fileBufferSize = (BYTE*)GlobalAlloc(LMEM_FIXED, 1024 * 1024);
	BYTE* filePosition = fileBufferSize;
	HANDLE file = CreateFile(arg_filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file != INVALID_HANDLE_VALUE)
	{
		enum {BUFFER_SIZE = 4096};
		BYTE readBuffer[BUFFER_SIZE];
		DWORD bytesRead = 0; 
		while (ReadFile(file, readBuffer, BUFFER_SIZE, &bytesRead, NULL) && (bytesRead != 0))
		{
			memcpy(filePosition, readBuffer, bytesRead);
			filePosition += bytesRead;
		}
		*filePosition = 0;
		if (filePosition != fileBufferSize)
		{
			SetWindowTextA(app.handle_window_edit, (char*)fileBufferSize);
			success = TRUE;
		}
		
		CloseHandle(file);
	}

	GlobalFree(fileBufferSize);
	return success;
}

BOOL MNoteSaveFile(const wchar_t* arg_filename)
{
	BOOL success = FALSE; 
	enum
	{
		BUFFER_SIZE = 1024 * 1024
	};

	char* fileBuffer = (char*)GlobalAlloc(GMEM_FIXED, BUFFER_SIZE);
	int length = GetWindowTextA(app.handle_window_edit, fileBuffer, BUFFER_SIZE);
	if (length != 0)
	{
		HANDLE file = CreateFile(arg_filename, GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file != INVALID_HANDLE_VALUE)
		{
			DWORD bytesWritten = 0;
			if (WriteFile(file, fileBuffer, length, &bytesWritten, NULL))
			{
				if (bytesWritten == length)
				{
					success = TRUE;
				}
			}
			CloseHandle(file);
		}
	}

	GlobalFree(fileBuffer);
	return success;
}

LRESULT CALLBACK MNoteWndProc(HWND arg_wnd, UINT arg_msg, WPARAM arg_param, LPARAM arg_lparam)
{
	switch (arg_msg)
	{
		case WM_CREATE:
		{
			app.handle_window_edit = CreateWindowEx(
				WS_EX_CLIENTEDGE, 
				L"EDIT", 
				L"", 
				WS_CHILD | ES_MULTILINE, 
				0, 0, 
				0, 0, 
				arg_wnd, 
				NULL, 
				app.handle_instance, 
				NULL);

			ShowWindow(app.handle_window_edit, SW_SHOW);
			ShowWindow(arg_wnd, SW_SHOW);
			break;
		}

		case WM_SIZE:
		{
			RECT rect;
			int width;
			int height;
			GetClientRect(arg_wnd, &rect);

			width = rect.right - rect.left;
			height = rect.bottom - rect.top;

			MoveWindow(app.handle_window_edit, 0, 0, width, height, FALSE);
			break;
		}

		case WM_COMMAND:
		{
			UINT id = LOWORD(arg_param);
			switch (id)
			{
				case ID_FILE_OPEN:
				{
					wchar_t filenameBuffer[MAX_PATH];
					ZeroMemory(&filenameBuffer, MAX_PATH * sizeof(wchar_t));
					OPENFILENAME ofn = { 0 };
					ofn.lStructSize = sizeof(ofn);
					ofn.hInstance = app.handle_instance;
					ofn.hwndOwner = arg_wnd;
					ofn.lpstrFileTitle = L"Open a file for us to read in MNote.";
					ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";
					ofn.lpstrFile = filenameBuffer;
					ofn.nMaxFile = MAX_PATH;
					
					if (GetOpenFileName(&ofn))
					{
						MNoteOpenFile(filenameBuffer);
					}
					break;
				}

				case ID_FILE_SAVE:
				{
					wchar_t filenameBuffer[MAX_PATH];
					ZeroMemory(&filenameBuffer, MAX_PATH * sizeof(wchar_t));
					OPENFILENAME ofn = { 0 };
					ofn.lStructSize = sizeof(ofn);
					ofn.hInstance = app.handle_instance;
					ofn.hwndOwner = arg_wnd;
					ofn.lpstrFileTitle = L"Save As";
					ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";
					ofn.lpstrFile = filenameBuffer;
					ofn.nMaxFile = MAX_PATH;

					if (GetSaveFileName(&ofn))
					{
						MNoteSaveFile(filenameBuffer);
					}
					break;
				}

				case ID_FILE_EXIT:
				{
					DestroyWindow(arg_wnd);
					break;
				}
			}
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(arg_wnd);
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(arg_wnd, arg_msg, arg_param, arg_lparam);
}

BOOL MNoteRegisterClass(HINSTANCE arg_instance)
{
	WNDCLASSEX wc = { 0 };

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MNoteWndProc;
	wc.hInstance = arg_instance;
	wc.lpszClassName = MNoteWindowClass;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	return (RegisterClassEx(&wc) != 0);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	if (!MNoteRegisterClass(hInstance))
	{
		MessageBox(NULL, L"Couldn't register the window class!", L"Error", MB_OKCANCEL | MB_ICONERROR);
	}

	app.handle_instance = hInstance;
	app.handle_window = CreateWindowEx(
		0, 
		MNoteWindowClass, 
		L"MNote", 
		WS_EX_OVERLAPPEDWINDOW, 
		0, 
		0, 
		680, 
		480, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}