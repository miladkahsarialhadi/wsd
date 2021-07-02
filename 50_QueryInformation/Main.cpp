#include "NTProcessInfo.h"

int main(int argc, char* argv[])
{
	MIAPROCESSINFO process_info;
	ULONG needed_size; 

	g_NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &process_info, sizeof(MIAPROCESSINFO), &needed_size);

	printf("Debug Status: %d", process_info.cBeingDebugged);

	return 0;
}