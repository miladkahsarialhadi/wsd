#include <windows.h>
#include <process.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

unsigned __stdcall FunctionOne(void* arg_list) 
{
	// Now all threads in a process will make low-priority I/O requests.
	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

	// Now primary thread issue low-priority I/O requests.
	SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);

	for (size_t i = 0; i < 1000; i++)
	{
		std::printf("\t FunctionOne, Repeat # %d\n", i+1);
		// Sleep(10);
		// std::this_thread::sleep_for(100ms);
		SwitchToThread();
	}

	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END);
	SetPriorityClass(GetCurrentProcess(), THREAD_MODE_BACKGROUND_END);
	return 0;
}

unsigned __stdcall FunctionTwo(void* arg_list)
{
	for (size_t i = 0; i < 1000; i++)
	{
		std::printf("\tFunctionTwo, Repeat # %d\n", i + 1);
		SwitchToThread();
	}
	return 0;
}

int main(int argc, char* argv[], char* envp[]) 
{
	HANDLE handle_thread1 = (HANDLE)_beginthreadex(NULL, 0, FunctionOne, NULL, 0, NULL);
	HANDLE handle_thread2 = (HANDLE)_beginthreadex(NULL, 0, FunctionTwo, NULL, 0, NULL);

	if (handle_thread1 == NULL && handle_thread2 == NULL)
	{
		return -1;
	}


	int recieve1 = WaitForSingleObject(handle_thread1, INFINITE); 
	int recieve2 = WaitForSingleObject(handle_thread2, INFINITE);

	CloseHandle(handle_thread1);
	CloseHandle(handle_thread2);

	return 0;
}