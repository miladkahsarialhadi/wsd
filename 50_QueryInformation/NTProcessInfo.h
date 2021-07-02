#pragma once

#include <windows.h>
#include <winternl.h>
#include <psapi.h>

#define STRSAFE_LIB
#include <strsafe.h>

#pragma comment(lib, "strsafe.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "psapi.lib")

#ifndef NTSTATUS
#define LONG NTSTATUS
#endif

// Unicode path usually prefix with '\\?\'
#define MAX_UNICODE_PATH	32767L

// Used in PEB struct
typedef ULONG MIAPPS_POST_PROCESS_INIT_ROUTINE;

// Used in PEB struct
typedef struct _MIAPEB_LDR_DATA {
	BYTE Reserved1[8];
	PVOID Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} MIAPEB_LDR_DATA, * MIAPPEB_LDR_DATA;

// Used in PEB struct
typedef struct _MIARTL_USER_PROCESS_PARAMETERS {
	BYTE Reserved1[16];
	PVOID Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} MIARTL_USER_PROCESS_PARAMETERS, * MIAPRTL_USER_PROCESS_PARAMETERS;

// Used in PROCESS_BASIC_INFORMATION struct
typedef struct _MIAPEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	MIAPPEB_LDR_DATA Ldr;
	MIAPRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	BYTE Reserved4[104];
	PVOID Reserved5[52];
	MIAPPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	BYTE Reserved6[128];
	PVOID Reserved7[1];
	ULONG SessionId;
} MIAPEB, * MIAPPEB;

// Used with NtQueryInformationProcess
typedef struct _MIAPROCESS_BASIC_INFORMATION {
	LONG ExitStatus;
	MIAPPEB PebBaseAddress;
	ULONG_PTR AffinityMask;
	LONG BasePriority;
	ULONG_PTR UniqueProcessId;
	ULONG_PTR InheritedFromUniqueProcessId;
} MIAPROCESS_BASIC_INFORMATION, * MIAPPROCESS_BASIC_INFORMATION;

// NtQueryInformationProcess in NTDLL.DLL
typedef NTSTATUS(NTAPI* pfnNtQueryInformationProcess)(
	IN	HANDLE ProcessHandle,
	IN	PROCESSINFOCLASS ProcessInformationClass,
	OUT	PVOID ProcessInformation,
	IN	ULONG ProcessInformationLength,
	OUT	PULONG ReturnLength	OPTIONAL
	);

pfnNtQueryInformationProcess g_NtQueryInformationProcess;

typedef struct _MIAPROCESSINFO
{
	DWORD	dwPID;
	DWORD	dwParentPID;
	DWORD	dwSessionID;
	DWORD	dwPEBBaseAddress;
	DWORD	dwAffinityMask;
	LONG	dwBasePriority;
	LONG	dwExitStatus;
	BYTE	cBeingDebugged;
	TCHAR	szImgPath[MAX_UNICODE_PATH];
	TCHAR	szCmdLine[MAX_UNICODE_PATH];
} MIAPROCESSINFO;

HMODULE LoadNTDLLFunctions(void);
void	FreeNTDLLFunctions(IN HMODULE arg_handle_ntdll);
BOOL	EnableTokenPrivilege(IN LPCTSTR arg_privilege);
BOOL	GetNtProcessInfo(IN const DWORD arg_pid, OUT MIAPROCESSINFO* arg_process_info);

