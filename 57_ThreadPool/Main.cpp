#include <windows.h>
#include <process.h>
#include <iostream>

PTP_WORK g_WorkItem = NULL;

VOID NTAPI WorkerOne(PTP_CALLBACK_INSTANCE arg_instance, PVOID arg_context)
{
	std::cout << "Thread ID: " << GetCurrentThreadId() << std::endl;
}

VOID NTAPI WorkerTwo(PTP_CALLBACK_INSTANCE arg_instance, PVOID arg_context)
{
	Sleep(500);
	std::cout << "Thread ID: " << GetCurrentThreadId() << std::endl;
}

int main(int argc, char* argv[])
{
	TrySubmitThreadpoolCallback(WorkerOne, NULL, NULL);
	TrySubmitThreadpoolCallback(WorkerTwo, NULL, NULL);

	WaitForThreadpoolWorkCallbacks(g_WorkItem, FALSE);

	return 0;
}