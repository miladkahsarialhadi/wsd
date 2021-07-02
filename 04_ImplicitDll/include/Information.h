#pragma once

#include <iostream>

class Information
{
private:
	std::string pi_name;
	std::string pi_family;
	int pi_age;

public:
	__declspec(dllexport) Information(std::string, std::string, int);
	void __declspec(dllexport) ShowInformation(void);
};

