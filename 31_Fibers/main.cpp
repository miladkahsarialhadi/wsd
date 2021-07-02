#include <windows.h>
#include <iostream>

void YieldHello(PVOID arg_fiber)
{
	std::cout << "Hello Universe, Milad is the most successful man." << std::endl;
	DeleteFiber(arg_fiber);
	ConvertFiberToThread();		// seize the main thread
}

int main(int argc, const char* argv[])
{
	HANDLE thread_to_fiber = ConvertThreadToFiber(0);
	HANDLE fiber_handle = CreateFiber(NULL, YieldHello, thread_to_fiber);
	SwitchToFiber(fiber_handle);
	abort(); /* unreachable */
	return 0;
}