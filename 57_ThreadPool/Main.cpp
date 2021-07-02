#include <Windows.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	double save_result = 0;
	HMODULE dll_load = LoadLibraryExW(L"Milad.dll", nullptr, 0);
	if (!dll_load)
	{
		cout << "We could not load the dynamic library" << endl;
		return EXIT_FAILURE;
	}
	using GetAddingType = double (WINAPI*)(double, double);
	GetAddingType DllLoadAdd = reinterpret_cast<GetAddingType>(GetProcAddress(dll_load, "?MPrint@Functions@@SANNN@Z"));
	if (DllLoadAdd) {
		cout << "We could not locate the function." << endl;
		return EXIT_FAILURE;
	}
	save_result = DllLoadAdd(22, 22);
	cout << "Function Returned: " << save_result << endl;

	FreeLibrary(dll_load);
	return EXIT_SUCCESS;
}