#include <windows.h>
#include <iostream>

int main(int argc, TCHAR* argv[])
{
	HANDLE handle_file = INVALID_HANDLE_VALUE;

	// Opens the existing file. 
	handle_file = CreateFile(L"Milad.txt",	// file name 
		GENERIC_WRITE,						// open for reading 
		0,									// do not share 
		NULL,								// default security 
		CREATE_ALWAYS,						// existing file only 
		FILE_ATTRIBUTE_NORMAL,				// normal file 
		NULL);								// no template 

	if (handle_file == INVALID_HANDLE_VALUE)
	{
		std::cout << "First CreateFile failed" << std::endl;
		return 1;
	}

	DWORD file_type = GetFileType(handle_file);

	switch (file_type)
	{
	case FILE_TYPE_UNKNOWN:
		std::cout << "The type of the specified file is unknown." << std::endl;
		break;
	case FILE_TYPE_DISK:
		std::cout << "The specified file is a disk file." << std::endl;
		break;
	case FILE_TYPE_CHAR:
		std::cout << "The specified file is a character file, typically an LPT device or a console." << std::endl;
		break;
	case FILE_TYPE_PIPE:
		std::cout << "The specified file is either a named pipe or an anonymous pipe." << std::endl;
		break;
	case FILE_TYPE_REMOTE:
		std::cout << "Unused." << std::endl;
		break;
	}

	return (0);
}
