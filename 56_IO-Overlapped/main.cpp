#include <windows.h>
#include <process.h>
#include <iostream>

wchar_t string_data[] = L"Garbage data is absolute thing";

unsigned WINAPI WorkerOne(PVOID arg_param)
{
	auto file_name = L"Milad.txt";
	auto handle_file = CreateFile(file_name, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);

	if (handle_file == INVALID_HANDLE_VALUE)
	{
		std::cout << "File creation is failed." << std::endl;
		return -1;
	}

	OVERLAPPED overlapped_instance = { 0 };

	// You don't need the null character at the end of the string to be written.
	if (!WriteFile(handle_file, string_data, sizeof(string_data) - 2, nullptr, &overlapped_instance))
	{
		auto status = GetLastError();
		if (status != ERROR_IO_PENDING)
		{
			std::cout << "WriteFile error " << status << "\n";
			return -1;
		}
		DWORD number_of_bytes;
		if (!GetOverlappedResult(handle_file, &overlapped_instance, &number_of_bytes, TRUE))
		{
			std::cout << "GetOverlappedResult error " << GetLastError() << "\n";
			return -1;
		}
		std::cout << "Completed: " << number_of_bytes << " bytes written.\n";
	}
	else
	{
		std::cout << "Completed immediately\n";
	}

	return 0;
}

unsigned WINAPI WorkerTwo(PVOID arg_param)
{
	for (int i = 0; i <= 1000; i++)
	{
		std::cout << "Loop: " << i << std::endl;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE handle_producer = (HANDLE)_beginthreadex(NULL, 0, WorkerOne, NULL, 0, NULL);
	HANDLE handle_consumer = (HANDLE)_beginthreadex(NULL, 0, WorkerTwo, NULL, 0, NULL);

	WaitForSingleObject(handle_producer, INFINITE);
	WaitForSingleObject(handle_consumer, INFINITE);

	CloseHandle(handle_producer);
	CloseHandle(handle_consumer);

	return 0;
}