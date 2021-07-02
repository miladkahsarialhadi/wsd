#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define BUFFER_SIZE				10
#define PRODUCER_SLEEP_TIME_MS	500
#define CONSUMER_SLEEP_TIME_MS	2000

ULONG				g_Buffer[BUFFER_SIZE];
ULONG				g_QueueSize;
ULONG				g_TotalItemsConsumed;
ULONG				g_TotalItemsProduced;
ULONG				g_LastItemProduced;
ULONG				g_QueueStartOffset;
CONDITION_VARIABLE	g_BufferFull;
CONDITION_VARIABLE	g_BufferEmpty;
CRITICAL_SECTION	g_BufferLock;
BOOL				g_StopRequested;

namespace mka
{
	namespace conditional_variable
	{
		DWORD WINAPI procedur(PVOID arg_param)
		{
			auto producer_id = (ULONG)(ULONG_PTR)arg_param;

			while (true)
			{
				// Produce a new item.
				Sleep(rand() % PRODUCER_SLEEP_TIME_MS);
				auto item = InterlockedIncrement(&g_LastItemProduced);
				EnterCriticalSection(&g_BufferLock);

				while (g_QueueSize == BUFFER_SIZE && g_StopRequested == FALSE)
				{
					// Buffer is full - sleep so consumers can get items.
					SleepConditionVariableCS(&g_BufferEmpty, &g_BufferLock, INFINITE);
				}

				if (g_StopRequested == TRUE)
				{
					LeaveCriticalSection(&g_BufferLock);
					break;
				}

				// Insert the item at the end of the queue and increment size.
				g_Buffer[(g_QueueStartOffset + g_QueueSize) % BUFFER_SIZE] = item;
				g_QueueSize++;
				g_TotalItemsProduced++;
				printf("Producer %u: item %2d, queue size %2u\r\n", producer_id, item, g_QueueSize);

				LeaveCriticalSection(&g_BufferLock);
				// If a consumer is waiting, wake it.
				WakeConditionVariable(&g_BufferFull);
			}

			printf("Producer %u exiting\r\n", producer_id);
			return 0;
		}

		DWORD WINAPI consumer(PVOID arg_param)
		{
			auto consumer_id = (ULONG)(ULONG_PTR)arg_param;
			while (true)
			{
				EnterCriticalSection(&g_BufferLock);
				while (g_QueueSize == 0 && g_StopRequested == FALSE)
				{
					// Buffer is empty - sleep so producers can create items.
					SleepConditionVariableCS(&g_BufferFull, &g_BufferLock, INFINITE);
				}

				if (g_StopRequested == TRUE && g_QueueSize == 0)
				{
					LeaveCriticalSection(&g_BufferLock);
					break;
				}

				// Consume the first available item.
				auto item = g_Buffer[g_QueueStartOffset];
				g_QueueSize--;
				g_QueueStartOffset++;
				g_TotalItemsConsumed++;

				if (g_QueueStartOffset == BUFFER_SIZE)
				{
					g_QueueStartOffset = 0;
				}

				printf("Consumer %u: item %2d, queue size %2u\r\n", consumer_id, item, g_QueueSize);
				LeaveCriticalSection(&g_BufferLock);
				// If a producer is waiting, wake it.
				WakeConditionVariable(&g_BufferEmpty);
				// Simulate processing of the item.
				Sleep(rand() % CONSUMER_SLEEP_TIME_MS);
			}

			printf("Consumer %u exiting\r\n", consumer_id);
			return 0;
		}
	}
}

int main(void)
{
	InitializeConditionVariable(&g_BufferEmpty);
	InitializeConditionVariable(&g_BufferFull);
	InitializeCriticalSection(&g_BufferLock);

	DWORD id;
	auto handle_producer = CreateThread(NULL, 0, mka::conditional_variable::procedur, (PVOID)1, 0, &id);
	auto handle_consumer = CreateThread(NULL, 0, mka::conditional_variable::consumer, (PVOID)1, 0, &id);

	puts("Press enter to stop...");
	getchar();

	EnterCriticalSection(&g_BufferLock);
	g_StopRequested = TRUE;
	LeaveCriticalSection(&g_BufferLock);

	HANDLE thread_handles[2] = { handle_producer, handle_consumer };

	auto thread_states = WaitForMultipleObjects(2, thread_handles, TRUE, INFINITE);

	printf("TotalItemsProduced: %u, TotalItemsConsumed: %u\r\n", g_TotalItemsProduced, g_TotalItemsConsumed);

	switch (thread_states)
	{
	case WAIT_OBJECT_0:
		std::cout << "Producer thread is finished." << std::endl;
	
	case WAIT_OBJECT_0 + 1:
		std::cout << "Consumer thread is finished." << std::endl;
		break;

	case WAIT_TIMEOUT:
		std::cout << "Timeout or a delay has been happened." << std::endl;
		break;

	case WAIT_FAILED:
		std::cout << "Threads have been failed." << std::endl;
		break;
	}

	return 0;
}