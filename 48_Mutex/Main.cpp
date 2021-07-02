#include <windows.h>
#include <iostream>

#define		THREADCOUNT 2
HANDLE		g_Mutex;

DWORD WINAPI WriteToDatabase(LPVOID arg_param)
{
	UNREFERENCED_PARAMETER(arg_param);
	DWORD count = 0;
	DWORD wait_result;

	// Request ownership of mutex.
	while (count < 20)
	{
		wait_result = WaitForSingleObject(g_Mutex, INFINITE);

		switch (wait_result)
		{
		case WAIT_OBJECT_0:
			// The thread got ownership of the mutex
			__try {
				std::printf("Thread %d writing to database...\n", GetCurrentThreadId());
				Sleep(1000);
				count++;
			}
			__finally {
				if (!ReleaseMutex(g_Mutex))
				{
					std::printf("We couldn't Release ownership of the mutex object.");
				}
			}
			break;

		case WAIT_ABANDONED:
			// The thread got ownership of an abandoned mutex	
			// The database is in an indeterminate state
			return FALSE;
		}
	}
	return TRUE;
}

int main(int argc, char* argv[])
{
	HANDLE	handle_thread[THREADCOUNT];
	DWORD	thread_identification;

	// Create a mutex with no initial owner
	g_Mutex = CreateMutex(NULL, FALSE, NULL);
	if (g_Mutex == NULL)
	{
		std::printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}

	// Create worker threads
	for (int i = 0; i < THREADCOUNT; i++)
	{
		handle_thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WriteToDatabase, NULL, 0, &thread_identification);
		if (handle_thread[i] == NULL)
		{
			std::printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}

	// Wait for all threads to terminate
	WaitForMultipleObjects(THREADCOUNT, handle_thread, TRUE, INFINITE);

	// Close thread and mutex handles
	for (int i = 0; i < THREADCOUNT; i++)
	{
		CloseHandle(handle_thread[i]);
	}

	CloseHandle(g_Mutex);
	
	return 0;
}