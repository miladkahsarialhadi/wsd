#include <Windows.h>
#include <iostream>

int main(int argc, const char* argv)
{
	std::wstring cMessage = L"Native Windows Development.\n";
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), cMessage.c_str(), cMessage.size(), NULL, NULL);

	return 0;
}