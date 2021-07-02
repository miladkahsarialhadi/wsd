#include <windows.h>
#include <iostream>

int main(int argc, char* argv[])
{
	// Now all threads in a process will make low-priority I/O requests.
	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

	// Now primary thread issue low-priority I/O requests.
	SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);

	DWORD  dw_priority_class = GetPriorityClass(GetCurrentProcess());
	std::printf("Priority class is 0x%x\n", dw_priority_class);

	DWORD  dw_priority_level = GetThreadPriority(GetCurrentThread());
	std::printf("Priority level is 0x%x\n", dw_priority_level);

	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END);
	SetPriorityClass(GetCurrentProcess(), THREAD_MODE_BACKGROUND_END);

	return 0;
}

