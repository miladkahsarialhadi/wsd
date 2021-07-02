#include <iostream>
#include <thread>
#include <atomic>
#include <windows.h>
#include <process.h>
#include <malloc.h>

long g_IntegerValue = 0;

typedef struct _PROGRAM_ITEM {
	SLIST_ENTRY m_ItemEntry;
	ULONG m_Signature;
} PROGRAM_ITEM, * PPROGRAM_ITEM;

void AtomicOperationOnStack()
{
	ULONG counter;
	PSLIST_ENTRY first_entry, list_entry;
	PSLIST_HEADER list_head;
	PPROGRAM_ITEM program_item;

	// Initialize the list header to a MEMORY_ALLOCATION_ALIGNMENT boundary.
	list_head = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT);

	if (NULL == list_head)
	{
		printf("Memory allocation failed.\n");
	}
	
	InitializeSListHead(list_head);

	// Insert 10 items into the list.
	for (counter = 1; counter <= 10; counter += 1)
	{
		program_item = (PPROGRAM_ITEM)_aligned_malloc(sizeof(PROGRAM_ITEM), MEMORY_ALLOCATION_ALIGNMENT);
		if (NULL == program_item)
		{
			printf("Memory allocation failed.\n");
		}
		program_item->m_Signature = counter;
		first_entry = InterlockedPushEntrySList(list_head, &(program_item->m_ItemEntry));
	}

	// Remove 10 items from the list and display the signature.
	for (counter = 10; counter >= 1; counter -= 1)
	{
		list_entry = InterlockedPopEntrySList(list_head);

		if (NULL == list_entry)
		{
			printf("List is empty.\n");
		}

		program_item = (PPROGRAM_ITEM)list_entry;
		printf("Signature is %d\n", program_item->m_Signature);

		// This example assumes that the SLIST_ENTRY structure is the 
		// first member of the structure. If your structure does not 
		// follow this convention, you must compute the starting address 
		// of the structure before calling the free function.

		_aligned_free(list_entry);
	}

	// Flush the list and verify that the items are gone.
	list_entry = InterlockedFlushSList(list_head);
	first_entry = InterlockedPopEntrySList(list_head);
	if (first_entry != NULL)
	{
		printf("Error: List is not empty.\n");
	}

	_aligned_free(list_head);
}

unsigned __stdcall WorkOne(PVOID arg_param)
{
	for (volatile size_t i = 0; i < 10000; i++)
	{
		 //g_IntegerValue++;
		 InterlockedAdd(&g_IntegerValue, 1);
		//InterlockedIncrement(&g_IntegerValue);
	}

	return 0;
}

unsigned __stdcall WorkTwo(PVOID arg_param)
{
	for (volatile size_t i = 0; i < 10000; i++)
	{
		 //g_IntegerValue++;
		InterlockedAdd(&g_IntegerValue, 1);
		//InterlockedIncrement(&g_IntegerValue);
	}
	return 0;
}

unsigned __stdcall WorkThree(PVOID arg_param)
{
	for (volatile size_t i = 0; i < 10000; i++)
	{
		 //g_IntegerValue++;
		InterlockedAdd(&g_IntegerValue, 1);
		//InterlockedIncrement(&g_IntegerValue);
	}
	return 0;
}

unsigned __stdcall WorkFour(PVOID arg_param)
{
	for (volatile size_t i = 0; i < 10000; i++)
	{
		 //g_IntegerValue++;
		 InterlockedAdd(&g_IntegerValue, 1);
		// InterlockedIncrement(&g_IntegerValue);
	}
	return 0;
}

void NativeAtomicOperation()
{
	HANDLE thread_one = (HANDLE)_beginthreadex(NULL, 0, WorkOne, NULL, 0, NULL);
	HANDLE thread_two = (HANDLE)_beginthreadex(NULL, 0, WorkTwo, NULL, 0, NULL);
	HANDLE thread_three = (HANDLE)_beginthreadex(NULL, 0, WorkThree, NULL, 0, NULL);
	HANDLE thread_four = (HANDLE)_beginthreadex(NULL, 0, WorkFour, NULL, 0 , NULL);

	WaitForSingleObject(thread_one, INFINITE);
	WaitForSingleObject(thread_two, INFINITE);
	WaitForSingleObject(thread_three, INFINITE);
	WaitForSingleObject(thread_four, INFINITE);

	std::cout << "The Final Value of Native Atomic Operation: " << g_IntegerValue << std::endl;
}

void StandardAtomicOperation()
{
	 std::atomic_int integer_value = 0;
	//int integer_value = 0;

	auto lambda_increament = [&]()
	{
		for (size_t i = 0; i < 10000; i++)
		{
			integer_value++;
		}
	};

	std::thread std_thread_one(lambda_increament);
	std::thread std_thread_two(lambda_increament);
	std::thread std_thread_thr(lambda_increament);
	std::thread std_thread_fur(lambda_increament);

	std_thread_one.join();
	std_thread_two.join();
	std_thread_thr.join();
	std_thread_fur.join();

	std::cout << "The Final Value of Standard Atomic Operation:: " << integer_value << std::endl;
}

int main(int argc, const char* argv)
{
	NativeAtomicOperation();
	//StandardAtomicOperation();

	return 0;
}