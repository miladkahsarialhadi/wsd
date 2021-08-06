#include <stdio.h>
#include <Windows.h>

#pragma comment(lib,"ntdll.lib")
#pragma comment(linker,"/include:__tls_used") 
#pragma section(".CRT$XLB",read) 

extern "C" NTSTATUS NTAPI NtQueryInformationProcess(HANDLE arg_process, ULONG arg_info_class, PVOID arg_buffer, ULONG arg_length, PULONG arg_return_length);

#define NtCurrentProcess() (HANDLE)-1

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

int main()
{
	getchar();

	return 0;
}
