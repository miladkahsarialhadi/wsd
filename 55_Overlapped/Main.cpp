#include <windows.h>
#include <iostream>

VOID WINAPI FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);

HANDLE g_HandleEvent;
wchar_t string_data[] = L"Garbage data is absolute thing";

int main(int argc, char* argv[])
{
	g_HandleEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	auto file_name = L"Cayot.txt";
	auto handle_file = CreateFile(file_name, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);

	if (handle_file == INVALID_HANDLE_VALUE)
	{
		std::cout << "File creation is failed." << std::endl;
		return -1;
	}

	OVERLAPPED overlapped_instance = { 0 };
	overlapped_instance.hEvent = g_HandleEvent;

	WriteFileEx(handle_file, string_data, sizeof(string_data), &overlapped_instance, FileIOCompletionRoutine);

	SleepEx(INFINITE, TRUE);

	return 0;
}

VOID WINAPI FileIOCompletionRoutine(DWORD arg_error_code, DWORD arg_numbers_bytes, LPOVERLAPPED arg_overlapped)
{
	std::cout << "Operation has been done." << std::endl;
}