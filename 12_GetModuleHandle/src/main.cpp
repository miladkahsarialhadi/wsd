#include <windows.h>
#include <iostream>

extern "C" const IMAGE_DOS_HEADER __ImageBase;

int wmain(int argc, const char* argv[])
{
	HMODULE hModule = GetModuleHandle(NULL);
	std::cout << "BaseImage with GetModuleHandle(NULL): " << hModule << std::endl;

	// Use the pseudo-variable __ImageBase to get the address of the current module hModule/hInstance.
	std::cout << "BaseImage with __ImageBase: " << (HINSTANCE)&__ImageBase << std::endl;

	// Pass the address of the current method Main as parameter to GetModuleHandleEx 
	// to get the address of the current module hModule/hInstance.

	hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (PCTSTR)wmain, &hModule);
	std::cout << "BaseImage Main with GetModuleHandleEx: " << hModule << std::endl;

	return 0;
}