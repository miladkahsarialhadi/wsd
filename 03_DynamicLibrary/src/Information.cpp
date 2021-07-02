#include "Information.h"


__declspec(dllexport) Information::Information(std::string arg_name, std::string arg_family, int arg_age) : pi_name(arg_name), pi_family(arg_family), pi_age(arg_age) {}

void __declspec(dllexport) Information::ShowInformation(void)
{
	std::cout << "\nYour name is " << pi_name << " " << pi_family << std::endl;
	std::cout << pi_name << " is " << pi_age << " year old." << std::endl;
}