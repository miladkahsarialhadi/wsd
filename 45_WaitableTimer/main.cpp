#include <windows.h>
#include <process.h>
#include <iostream>

HANDLE g_handleTimer = NULL;

unsigned WINAPI WaitDuration(PVOID arg_param)
{
	SYSTEMTIME system_time;
	FILETIME file_time_local, file_time_utc;
	LARGE_INTEGER li_utc;
	
	// First signaling is at February 1, 2021, at 3:00 P.M. (local time).
	system_time.wYear = 2021;		// Year
	system_time.wMonth = 2;			// February
	system_time.wDayOfWeek = 0;		// Ignored
	system_time.wDay = 28;			// The first of the month
	system_time.wHour = 19;			// 3PM
	system_time.wMinute = 33;		// 0 minutes into the hour
	system_time.wSecond = 0;		// 0 seconds into the minute
	system_time.wMilliseconds = 0;	// 0 milliseconds into the second
	SystemTimeToFileTime(&system_time, &file_time_local);
	
	// Convert local time to UTC time.
	LocalFileTimeToFileTime(&file_time_local, &file_time_utc);
	
	// Convert FILETIME to LARGE_INTEGER because of different alignment.
	li_utc.LowPart = file_time_utc.dwLowDateTime;
	li_utc.HighPart = file_time_utc.dwHighDateTime;
	
	// Set the timer.
	SetWaitableTimer(g_handleTimer, &li_utc, 0, NULL, NULL, FALSE);

	return 0;
}

unsigned WINAPI PrintMessageWithInterval(PVOID arg_param)
{
	WaitForSingleObject(g_handleTimer, INFINITE);

	std::cout << "I have received the signal." << std::endl;

	return 0;
}

int main(int argc, const char argv[])
{
	// Create an auto-reset timer.
	g_handleTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	HANDLE handle_producer = (HANDLE)_beginthreadex(NULL, 0, WaitDuration, NULL, 0, NULL);
	HANDLE handle_consumer = (HANDLE)_beginthreadex(NULL, 0, PrintMessageWithInterval, NULL, 0, NULL);

	WaitForSingleObject(handle_producer, INFINITE);
	WaitForSingleObject(handle_consumer, INFINITE);

	CloseHandle(g_handleTimer);
	CloseHandle(handle_producer);
	CloseHandle(handle_consumer);


	return 0;
}