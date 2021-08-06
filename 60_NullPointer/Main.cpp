#include <windows.h>
#include <iostream>

int main(int argc, char* argv[])
{
	// VirtualAlloc return a LPVOID, so we should cast it exactly to what we wanted to use.
	// If we don't commit the memory, we get access violation

	// 0x00000000
	auto memory = (int*)VirtualAlloc(NULL, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	*memory = 20000;
	std::printf("%d", *memory);
	VirtualFree(memory, 0, MEM_RELEASE);

	return 0;
}