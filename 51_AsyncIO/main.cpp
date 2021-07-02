#include <windows.h>
#include <iostream>

int main(int argc, const char* argv)
{
	HANDLE handle_file = CreateFile(L"Milad.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_FLAG_OVERLAPPED, 
		NULL);

	BYTE data_buffer[100];
	OVERLAPPED overlapped = { 0 };
	overlapped.Offset = 345;

	BOOL read_done = ReadFile(handle_file, data_buffer, 100, NULL, &overlapped);
	DWORD error_code = GetLastError();

	if (!read_done && (error_code == ERROR_IO_PENDING)) {
		// The I/O is being performed asynchronously; wait for it to complete
		WaitForSingleObject(handle_file, INFINITE);
		read_done = TRUE;
	}

	if (read_done) {
		// overlapped.Internal contains the I/O error
		// overlapped.InternalHigh contains the number of bytes transferred
		// data_buffer contains the read data
	}
	else {
		std::cout << "Error happened: " << GetLastError() << std::endl;
	}

	return 0;
}