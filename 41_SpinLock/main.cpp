#include <Windows.h>
#include <thread>
#include <iostream>
#include <mutex>

CRITICAL_SECTION g_CriticalSection;
volatile int g_Counter = 100000;
int g_Sum = 0;

struct spinlock
{
	std::atomic<bool> m_lock = { 0 };

	void lock() noexcept
	{
		for (;;)
		{
			// Optimistically assume the lock is free on the first try
			if (!m_lock.exchange(true, std::memory_order_acquire))
			{
				return;
			}
			// Wait for lock to be released without generating cache misses
			while (m_lock.load(std::memory_order_relaxed))
			{
				// Issue X86 PAUSE or ARM YIELD instruction to reduce contention between hyper-threads
				_mm_pause();
			}
		}
	}

	bool try_lock() noexcept
	{
		// First do a relaxed load to check if lock is free in order to prevent
		// unnecessary cache misses if someone does while(!try_lock())
		return !m_lock.load(std::memory_order_relaxed) && !m_lock.exchange(true, std::memory_order_acquire);
	}

	void unlock() noexcept
	{
		m_lock.store(false, std::memory_order_release);
	}
};

DWORD WINAPI FirstWorker(PVOID arg_param)
{
	InitializeCriticalSectionAndSpinCount(&g_CriticalSection, 1000);
	EnterCriticalSection(&g_CriticalSection);
	g_Sum = 0;
	for (int n = 1; n <= g_Counter; n++)
	{
		g_Sum += n;
	}

	// Decrease lock count of RTL_CRITICAL_SECTION
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
	auto thread_handle_1 = CreateThread(NULL, 0, FirstWorker, NULL, NULL, NULL);
	auto thread_handle_2 = CreateThread(NULL, 0, SecondWorker, NULL, NULL, NULL);

	WaitForSingleObject(thread_handle_1, INFINITE);
	WaitForSingleObject(thread_handle_2, INFINITE);

	std::cout << "Final Result: " << g_Sum << std::endl;

	return 0;
}