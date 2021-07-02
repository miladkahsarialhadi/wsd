#include <windows.h>
#include <iostream>
#include <bitset>

DWORD GetThreadAffinityMask(HANDLE arg_thread)
{
	DWORD mask = 1;
	DWORD old = 0;

	// try every CPU one by one until one works or none are left
	while (mask)
	{
		old = SetThreadAffinityMask(arg_thread, mask);
		if (old)
		{   
			// this one worked
			SetThreadAffinityMask(arg_thread, old); 
			
			// restore original
			return old;
		}
		else
		{
			if (GetLastError() != ERROR_INVALID_PARAMETER)
				return 0; // fatal error, might as well throw an exception
		}
		mask <<= 1;
	}

	return 0;
}

int main(int argc, const char* argv[])
{
	auto shared_user_data = (BYTE*)0x7FFE0000;
	printf("Version: %d.%d.%d\n",
		*(ULONG*)(shared_user_data + 0x26c), // major version offset
		*(ULONG*)(shared_user_data + 0x270), // minor version offset
		*(ULONG*)(shared_user_data + 0x260)); // build number offset (Windows 10)

	SYSTEM_INFO system_info;
	::GetSystemInfo(&system_info);
	std::cout << "Number of CPU cores: " << system_info.dwNumberOfProcessors << std::endl;
	std::cout << "Processor Mask: " << "0x" << std::bitset<8>(system_info.dwActiveProcessorMask) << std::endl;


	SetProcessAffinityMask(GetCurrentProcess(), 0x00000005);
	
	DWORD_PTR process_affinity_mask;
	DWORD_PTR system_affinity_mask;
	GetProcessAffinityMask(GetCurrentProcess(), &process_affinity_mask, &system_affinity_mask);

	std::cout << "Process Affinity: " << "0x" << std::bitset<8>(process_affinity_mask) << std::endl;
	std::cout << "System Affinity: " << "0x" << std::bitset<8>(process_affinity_mask) << std::endl;

	std::cout << "Primary Thread Affinity: " << "0x" << std::bitset<8>(GetThreadAffinityMask(GetCurrentThread())) << std::endl;

	return 0;
}