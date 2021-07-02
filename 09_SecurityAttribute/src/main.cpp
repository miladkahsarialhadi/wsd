#include <windows.h>
#include <iostream>

void CheckErrorCode(const DWORD arg_error_code)
{
	char ccErrorDescription[256];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		arg_error_code,
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
}

int main(int argc, const char* argv[])
{
	SECURITY_ATTRIBUTES stFileSecurityAttribute;

	stFileSecurityAttribute.nLength = sizeof(stFileSecurityAttribute);
	stFileSecurityAttribute.lpSecurityDescriptor = NULL;
	stFileSecurityAttribute.bInheritHandle = FALSE;

	CreateFile("Sample.txt", 0, 0, &stFileSecurityAttribute, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	CheckErrorCode(GetLastError());

	return 0;
}