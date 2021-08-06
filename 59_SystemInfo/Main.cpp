#include <windows.h>
#include <iostream>

void ShowProcessors() 
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION cpu_info_buffer = NULL;
	DWORD size = 0;
	DWORD cpu_core_count;

	BOOL op_result = GetLogicalProcessorInformation(cpu_info_buffer, &size);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
	{
		std::printf("Impossible to get processor information\n");
		return;
	}
	cpu_info_buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(size);
	op_result = GetLogicalProcessorInformation(cpu_info_buffer, &size);
	if (!op_result) 
	{
		free(cpu_info_buffer);
		std::printf("Impossible to get processor information\n");
		return;
	}

	cpu_core_count = 0;
	DWORD lpi_count = size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
	for (DWORD current = 0; current < lpi_count; current++) 
	{
		if (cpu_info_buffer[current].Relationship == RelationProcessorCore) 
		{
			if (cpu_info_buffer[current].ProcessorCore.Flags == 1) 
			{
				std::printf(" + one CPU core (HyperThreading)\n");
			}
			else 
			{
				std::printf(" + one CPU socket\n");
			}
			cpu_core_count++;
		}
	}
	std::printf(" -> %d active CPU(s)\n", cpu_core_count);
	free(cpu_info_buffer);
}

int main(int argc, char* argv[])
{
	ShowProcessors();

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	std::cout << "Page Size: " << si.dwPageSize / 1024 << " kb." << std::endl;
	std::cout << "Granularity Size: " << si.dwAllocationGranularity / 1024 << " kb." << std::endl;

	BOOL wow64_environment = FALSE;
	IsWow64Process(GetCurrentProcess(), &wow64_environment);
	std::cout << "Wow64 Environment: " << wow64_environment << std::endl;

	MEMORYSTATUS memory_status;
	memory_status.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memory_status);
	std::cout << "Total Page File: " << memory_status.dwTotalPageFile / 1014 << " gb." << std::endl;
	std::cout << "Available Page File: " << memory_status.dwAvailPageFile / 1014 << " gb." << std::endl;

	return 0;
}