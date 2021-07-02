#include "CLib.h"

void __declspec(dllexport) SetInformation(std::string arg_name, std::string arg_family, int arg_age) {
	g_name = arg_name;
	g_family = arg_family;
	g_age = arg_age;
}

void __declspec(dllexport) GetInformation(void)
{
	std::cout << "\nYour name is " << g_name << " " << g_family << std::endl;
	std::cout << g_name << " is " << g_age << " year old." << std::endl;
}