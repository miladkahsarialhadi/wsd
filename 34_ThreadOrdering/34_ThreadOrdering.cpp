#include <windows.h>
#include <process.h>
#include <iostream>
#include <avrt.h>
#include <stdio.h>

#pragma comment(lib, "Avrt.lib")
#define _100NS_IN_1MS 10000

unsigned __stdcall Thread1(void* arg_list)
{
	for (size_t i = 0; i < 100; i++)
	{
		std::cout << "\tHello from Thread1, Repeat #" << i + 1 << std::endl;
		SwitchToThread();
	}
	return 0;
}

unsigned __stdcall Thread2(void* arg_list)
{
	for (size_t i = 0; i < 100; i++)
	{
		std::cout << "\tHello from Thread2, Repeat #" << i + 1 << std::endl;
	}
	return 0;
}


int main()
{
	HANDLE handle_context = NULL;
	LARGE_INTEGER period, timeout;
	GUID guid = { 0 };
	BOOL result;

	period.QuadPart = Int32x32To64(_100NS_IN_1MS, 1000);   // 1 second
	timeout.QuadPart = Int32x32To64(_100NS_IN_1MS, 10000); // 10 seconds

	result = AvRtCreateThreadOrderingGroup(&handle_context, &period, &guid, &timeout);

	if (result != TRUE)
	{
		printf("Error creating group (%d)\n", GetLastError());
		return 1;
	}

	AvRtJoinThreadOrderingGroup(&handle_context, &guid, TRUE);
	AvRtWaitOnThreadOrderingGroup(handle_context);
	AvRtLeaveThreadOrderingGroup(handle_context);
	AvRtDeleteThreadOrderingGroup(handle_context);

	HANDLE handle_thread1 = (HANDLE)_beginthreadex(NULL, 0, Thread1, NULL, CREATE_SUSPENDED, NULL);
	HANDLE handle_thread2 = (HANDLE)_beginthreadex(NULL, 0, Thread2, NULL, CREATE_SUSPENDED, NULL);
	
	if (handle_thread1 == NULL && handle_thread2 == NULL)
	{
		return -1;
	}

	ResumeThread(handle_thread1);
	ResumeThread(handle_thread2);

	int recieve1 = WaitForSingleObject(handle_thread1, INFINITE);
	int recieve2 = WaitForSingleObject(handle_thread2, INFINITE);

	CloseHandle(handle_thread1);
	CloseHandle(handle_thread2);	
}