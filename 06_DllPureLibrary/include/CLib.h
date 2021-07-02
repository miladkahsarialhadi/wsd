#pragma once

#include <iostream>

extern "C"
{
	std::string g_name;
	std::string g_family;
	int g_age;

	void __declspec(dllexport) SetInformation(std::string, std::string, int);
	void __declspec(dllexport) GetInformation(void);
}