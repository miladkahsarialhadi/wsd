#include <windows.h>
#include <process.h>
#include <iostream>

int g_RepeatNumber = 10;
PTP_WORK g_WorkItem = NULL;
HANDLE g_Event = NULL;

FILETIME MakeFileTime(long long arg_time)
{
	FILETIME ft = {};
	ft.dwLowDateTime = static_cast<DWORD>(arg_time);
	ft.dwHighDateTime = static_cast<DWORD>(arg_time >> 32);
	return ft;
}

VOID CALLBACK WorkerOne(PTP_CALLBACK_INSTANCE arg_instance, PVOID arg_context, PTP_TIMER arg_timer)
{
	std::printf("Count: %d\n", g_RepeatNumber);
	--g_RepeatNumber;

	if (g_RepeatNumber == 0)
		SetEvent(g_Event);
}

int main(int argc, char* argv[])
{
	g_Event = CreateEvent(NULL, TRUE, FALSE, NULL);

	PTP_TIMER pointer_timer = CreateThreadpoolTimer(WorkerOne, NULL, NULL);

	if (pointer_timer == NULL)
	{
		std::cout << "Error: something goes wrong." << std::endl;
		return -1;
	}

	auto ft_relative_start_time = MakeFileTime(-10000000);;

	SetThreadpoolTimer(pointer_timer, &ft_relative_start_time, 1000, 0);

	if (IsThreadpoolTimerSet(pointer_timer))
	{
		std::cout << "Time has been set." << std::endl;
	}
	else
	{
		std::cout << "Error: something bad happened." << std::endl;
	}

	if (WaitForSingleObject(g_Event, INFINITE))
	{
		CloseThreadpoolTimer(pointer_timer);
		return 0;
	}
}