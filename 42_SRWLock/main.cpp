#include <Windows.h>
#include <thread>
#include <iostream>
#include <mutex>

RTL_SRWLOCK g_SrwLock;
volatile int g_Counter = 100000;
int g_Sum = 0;

namespace mka
{
	namespace rw_lock
	{
		DWORD WINAPI writer(PVOID arg_param)
		{
			InitializeSRWLock(&g_SrwLock);
			AcquireSRWLockExclusive(&g_SrwLock);
			g_Sum = 0;
			for (int n = 1; n <= g_Counter; n++)
			{
				g_Sum += n;
			}
			ReleaseSRWLockExclusive(&g_SrwLock);
			return 0;
		}

		DWORD WINAPI reader(PVOID arg_param)
		{
			AcquireSRWLockShared(&g_SrwLock);
			std::cout << "Final: " << g_Sum << std::endl;
			ReleaseSRWLockShared(&g_SrwLock);
			return 0;
		}
	}
}

int main(int argc, char* argv[])
{
	auto thread_handle_1 = CreateThread(NULL, 0, mka::rw_lock::writer, NULL, NULL, NULL);
	auto thread_handle_2 = CreateThread(NULL, 0, mka::rw_lock::reader, NULL, NULL, NULL);

	WaitForSingleObject(thread_handle_1, INFINITE);
	WaitForSingleObject(thread_handle_2, INFINITE);

	return 0;
}