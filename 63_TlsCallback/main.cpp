#include <stdio.h>
#include <Windows.h>

#pragma comment(lib,"ntdll.lib")

// This will cause the linker to create the TLS directory
#pragma comment(linker,"/include:__tls_used") 

// Create a new section
#pragma section(".CRT$XLB",read) 

extern "C" NTSTATUS NTAPI NtQueryInformationProcess(HANDLE arg_process, ULONG arg_info_class, PVOID arg_buffer, ULONG arg_length, PULONG arg_return_length);

#define NtCurrentProcess() (HANDLE)-1

// The TLS callback is called before the process entry point executes, and is executed before the debugger breaks
// This allows you to perform anti-debugging checks before the debugger can do anything
// Therefore, TLS callback is a very powerful anti-debugging technique

void WINAPI TlsCallback(PVOID arg_module, DWORD arg_reason, PVOID arg_context)
{
	PBOOLEAN being_debugged = (PBOOLEAN)__readfsdword(0x30) + 2;
	HANDLE debug_port = NULL;

	if (*being_debugged) // Read the PEB
	{
		MessageBox(NULL, L"Debugger detected!", L"TLS callback", MB_ICONSTOP);
	}

	else
	{
		MessageBox(NULL, L"No debugger detected", L"TLS callback", MB_ICONINFORMATION);
	}

	// Another check

	if (!NtQueryInformationProcess(NtCurrentProcess(), 7, &debug_port, sizeof(HANDLE), NULL))
	{
		if (debug_port)
		{
			MessageBox(NULL, L"Debugger detected!", L"TLS callback", MB_ICONSTOP);
		}

		else
		{
			MessageBox(NULL, L"No debugger detected", L"TLS callback", MB_ICONINFORMATION);
		}
	}
}

__declspec(allocate(".CRT$XLB")) PIMAGE_TLS_CALLBACK CallbackAddress[] = { TlsCallback,NULL }; 
// Put the TLS callback address into a null terminated array of the .CRT$XLB section
// The entry point is executed after the TLS callback

int main()
{
	printf("Hello world");
	getchar();

	return 0;
}