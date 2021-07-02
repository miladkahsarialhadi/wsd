#include <iostream>
#include <windows.h>

class Memory
{
public:
	Memory()
	{
		std::cout << "Memory constructor is called." << std::endl;
	}

	~Memory()
	{
		std::cout << "Memory destructor is called." << std::endl;
	}
};

Memory g_o_Memory;

int main(int argc, const char* argv[])
{
	{
		Memory l_o_Memory;
	}

	return 0;
}