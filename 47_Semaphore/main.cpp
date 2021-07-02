#include <windows.h>
#include <stdio.h>

#define MAX_SEM_COUNT	12
#define THREADCOUNT		12

HANDLE g_HandleSemaphore;

DWORD WINAPI Work(LPVOID arg_param)
{
	// lpParam not used in this example
	UNREFERENCED_PARAMETER(arg_param);

	DWORD	wait_result;
	BOOL	status_continue = TRUE;

	while (status_continue)
	{
		// Try to enter the semaphore gate with 0 second time interval.
		wait_result = WaitForSingleObject(g_HandleSemaphore, 0L);

		switch (wait_result)
		{
		case WAIT_OBJECT_0:
			// The semaphore object was signaled.
			printf("Thread %d scheduled\n", GetCurrentThreadId());
			status_continue = FALSE;

			// Simulate thread spending time on task
			Sleep(5);

			// Release the semaphore when task is finished
			// increase count by one and not interested in previous count
			if (!ReleaseSemaphore(g_HandleSemaphore, 1, NULL))
			{
				printf("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;

		case WAIT_TIMEOUT:
			// The semaphore was non signaled, so a time-out occurred.
			printf("Thread %d is waiting ...\n", GetCurrentThreadId());
			break;
		}
	}
	return TRUE;
}

int main(int argc, char* argv[])
{
	HANDLE	handle_thread[THREADCOUNT];
	DWORD	thread_identification;

	// Create an unnamed semaphore with initial and max counts of MAX_SEM_COUNT
	g_HandleSemaphore = CreateSemaphore(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, NULL);
	if (g_HandleSemaphore == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}

	// Create worker threads
	for (int i = 0; i < THREADCOUNT; i++)
	{
		handle_thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Work, NULL, 0, &thread_identification);
		if (handle_thread[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}

	// Wait for all threads to terminate
	WaitForMultipleObjects(THREADCOUNT, handle_thread, TRUE, INFINITE);

	// Close thread and semaphore handles
	for (int i = 0; i < THREADCOUNT; i++)
	{
		CloseHandle(handle_thread[i]);
	}

	CloseHandle(g_HandleSemaphore);
	return 0;
}

