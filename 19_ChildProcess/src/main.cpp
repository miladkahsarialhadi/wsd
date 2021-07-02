#include <Windows.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hFileCreatedInheritance = INVALID_HANDLE_VALUE;
	std::string sLine;
	std::ifstream infile("E:\\Sample.txt");

	if (infile.good())
	{
		getline(infile, sLine); // Get first line containing handle of file
		infile.close();

		unsigned int i = std::stoi(sLine.c_str(), 0, 16); // Convert hexadecimal string

		hFileCreatedInheritance = (HANDLE)i; // cast to a HANDLE
		printf("The handle of the file is %p.\n", hFileCreatedInheritance);
	}


	return 0;
}