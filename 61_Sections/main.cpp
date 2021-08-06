#include <windows.h>
#include <iostream>

#pragma data_seg("Shared")
volatile LONG g_ApplicationInstances = 0;
#pragma data_seg()

#pragma comment(linker, "/Section:Shared,RWS")

int main(int argc, char* argv[])
{
	InterlockedExchangeAdd(&g_ApplicationInstances, 1);

	std::cout << "Instance: " << g_ApplicationInstances << std::endl;

	std::cin.get();

	InterlockedExchangeAdd(&g_ApplicationInstances, -1);

	return 0;
}