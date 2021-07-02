#include <windows.h>
#include <iostream>
#include <winerror.h>

int main(int argc, const char* argv[]) 
{

	CreateFile("Sample.txt", 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	char ccErrorDescription[256];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		0,
		ccErrorDescription,
		255,
		NULL
		);

	if (GetLastError() != 0)
	{
		std::cout << "ERROR: " << ccErrorDescription << std::endl;
	}
	else
	{
		std::cout << "SUCESS: " << ccErrorDescription << std::endl;
	}
	return 0;
}