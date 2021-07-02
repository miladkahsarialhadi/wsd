#include <Windows.h>
#include <thread>
#include <iostream>
#include <mutex>

CRITICAL_SECTION g_CriticalSection;
volatile int g_Counter = 100000;
int g_Sum = 0;

DWORD WINAPI FirstWorker(PVOID arg_param)
{
	InitializeCriticalSection(&g_CriticalSection);
	EnterCriticalSection(&g_CriticalSection);
	g_Sum = 0;
	for (int n = 1; n <= g_Counter; n++) 
	{
		g_Sum += n;
	}

	// Decrese lock count of RTL_CRITICAL_SECTION
	LeaveCriticalSection(&g_CriticalSection);
	return 0;
}

DWORD WINAPI SecondWorker(PVOID arg_param)
{
	// False: Shared resource is locked.
	// True: Shared resource isn't locked.
	if (TryEnterCriticalSection(&g_CriticalSection))
	{
		g_Sum = 0;
		for (int n = 1; n <= g_Counter; n++)
		{
			g_Sum += n;
		}
		LeaveCriticalSection(&g_CriticalSection);
		DeleteCriticalSection(&g_CriticalSection);
	}
	else
	{
		std::cout << "Shared resources has been locked." << std::endl;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE thread_handle_1 = CreateThread(NULL, 0, FirstWorker, NULL, NULL, NULL);
	HANDLE thread_handle_2 = CreateThread(NULL, 0, SecondWorker, NULL, NULL, NULL);

	WaitForSingleObject(thread_handle_1, INFINITE);
	WaitForSingleObject(thread_handle_2, INFINITE);

	std::cout << "Final Result: " << g_Sum << std::endl;

	return 0;
}