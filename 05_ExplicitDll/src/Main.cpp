#include <Windows.h>
#include <iostream>
#include <functional>

typedef void(*SetInformation)(std::string, std::string, int);
typedef void(*GetInformation)(void);

int main(int argc, const char* argv[])
{
	std::function<void(std::string, std::string, int)> SetInformationInstance;
	std::function<void(void)> GetInformationInstance;

	HINSTANCE HandleDllLibrary = LoadLibrary(TEXT("06_DllPureLibrary.dll"));
	if (!HandleDllLibrary)
	{
		std::cout << "Dll isn't loaded successfully." << std::endl;
	}
	else
	{
		SetInformationInstance = reinterpret_cast<SetInformation>(GetProcAddress(HandleDllLibrary, "SetInformation"));
		GetInformationInstance = reinterpret_cast<GetInformation>(GetProcAddress(HandleDllLibrary, "GetInformation"));
		if (!GetInformationInstance || !GetInformationInstance)
		{
			std::cout << "Functions didn't resolved successfully." << std::endl;
		}
		else
		{
			SetInformationInstance("Milad", "Kahsari Alhadi", 25);
			GetInformationInstance();
		}
	}

	FreeLibrary(HandleDllLibrary);
	return 0;
}