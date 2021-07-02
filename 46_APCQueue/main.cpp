#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>

#define _SECOND 10000000

typedef struct _MYDATA {
	char*	m_text;
	DWORD	m_value;
} MYDATA;

VOID CALLBACK TimerAPCProcedure(LPVOID arg_ptr, DWORD arg_timer_low_value, DWORD arg_timer_high_value)
{
	// Formal parameters not used in this example.
	UNREFERENCED_PARAMETER(arg_timer_low_value);
	UNREFERENCED_PARAMETER(arg_timer_high_value);

	auto OInformation = static_cast<MYDATA*>(arg_ptr);

	std::printf("Message: %s\nValue: %d\n\n", OInformation->m_text, OInformation->m_value);
}

int main(int argc, const char* argv[])
{
	HANDLE          timer;
	BOOL            sucess;
	__int64         due_time;
	LARGE_INTEGER   large_due_time;
	MYDATA          OData;

	OData.m_text = const_cast<char*>("This is my data");
	OData.m_value = 100;
	timer = CreateWaitableTimer(NULL, FALSE, L"MyTimer");

	if (timer != NULL)
	{
		__try
		{
			// Create an integer that will be used to signal the timer 5 seconds from now.
			due_time = -5 * _SECOND;

			// Copy the relative time into a LARGE_INTEGER.
			large_due_time.LowPart = (DWORD)(due_time & 0xFFFFFFFF);
			large_due_time.HighPart = (LONG)(due_time >> 32);

			sucess = SetWaitableTimer(timer, &large_due_time, 2000, TimerAPCProcedure, &OData, FALSE);

			if (sucess)
			{
				for (; OData.m_value < 1000; OData.m_value += 100)
				{
					SleepEx(INFINITE, TRUE);
				}
			}
			else
			{
				printf("SetWaitableTimer failed with error %d\n", GetLastError());
			}
		}
		__finally
		{
			CloseHandle(timer);
		}
	}
	else
	{
		printf("CreateWaitableTimer failed with error %d\n", GetLastError());
	}
	return 0;
}