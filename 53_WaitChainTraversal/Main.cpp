/*
 * Sample code for the Wait Chain Traversal (WCT) API.
 *
 * This program enumerates all threads in the system and prints the
 * wait chain for each of them.  It should be run from an elevated
 * command prompt to get results for services.
 *
 */

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wct.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Advapi32.lib")

typedef struct _STR_ARRAY
{
    CHAR m_description[32];
} STR_ARRAY;

// Human-readable names for the different synchronization types.
STR_ARRAY STR_OBJECT_TYPE[] =
{
    {"CriticalSection"},
    {"SendMessage"},
    {"Mutex"},
    {"Alpc"},
    {"Com"},
    {"ThreadWait"},
    {"ProcWait"},
    {"Thread"},
    {"ComActivation"},
    {"Unknown"},
    {"Max"}
};

// Global variable to store the WCT session handle
HWCT g_WctHandle = NULL;

// Global variable to store OLE32.DLL module handle.
HMODULE g_Ole32Hnd = NULL;

//
// Function prototypes
//

void PrintWaitChain(__in DWORD ThreadId);


/*++

Routine Description:

    Enables the debug privilege (SE_DEBUG_NAME) for this process.
    This is necessary if we want to retrieve wait chains for processes
    not owned by the current user.

Arguments:

    None.

Return Value:

    TRUE if this privilege could be enabled; FALSE otherwise.

--*/

BOOL GrantDebugPrivilege()
{
    BOOL             success = FALSE;
    HANDLE           token_handle = NULL;
    TOKEN_PRIVILEGES token_privilege;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token_handle))
    {
        printf("Could not get the process token");
        goto Cleanup;
    }

    token_privilege.PrivilegeCount = 1;

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &token_privilege.Privileges[0].Luid))
    {
        printf("Couldn't lookup SeDebugPrivilege name");
        goto Cleanup;
    }

    token_privilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(token_handle, FALSE, &token_privilege, sizeof(token_privilege), NULL, NULL))
    {
        printf("Could not revoke the debug privilege");
        goto Cleanup;
    }

    success = TRUE;

Cleanup:

    if (token_handle)
    {
        CloseHandle(token_handle);
    }

    return success;
}

/*++

Routine Description:

    Enumerates all threads (or optionally only threads for one
    process) in the system.  It the calls the WCT API on each of them.

Arguments:

    ProcId--Specifies the process ID to analyze.  If '0' all processes
        in the system will be checked.

Return Value:

    TRUE if processes could be checked; FALSE if a general failure
    occurred.

--*/

BOOL CheckThreads(__in DWORD arg_process_id)
{
    DWORD processes[1024];
    DWORD processes_numbers;

    // Try to enable the SE_DEBUG_NAME privilege for this process. 
    // Continue even if this fails--we just won't be able to retrieve
    // wait chains for processes not owned by the current user.
    if (!GrantDebugPrivilege())
    {
        printf("Could not enable the debug privilege");
    }

    // Get a list of all processes currently running.
    if (EnumProcesses(processes, sizeof(processes), &processes_numbers) == FALSE)
    {
        printf("Could not enumerate processes");
        return FALSE;
    }

    for (DWORD index = 0; index < processes_numbers / sizeof(DWORD); index++)
    {
        HANDLE process;
        HANDLE snapshot;

        if (processes[index] == GetCurrentProcessId())
        {
            continue;
        }

        // If the caller specified a Process ID, check if we have a match.
        if (arg_process_id != 0)
        {
            if (processes[index] != arg_process_id)
            {
                continue;
            }
        }

        // Get a handle to this process.
        process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[index]);
        if (process)
        {
            WCHAR file[MAX_PATH];

            printf("Process 0x%x - ", processes[index]);

            // Retrieve the executable name and print it.
            if (GetProcessImageFileName(process, file, ARRAYSIZE(file)) > 0)
            {
                PCWSTR filePart = wcsrchr(file, L'\\');
                if (filePart)
                {
                    filePart++;
                }
                else
                {
                    filePart = file;
                }

                printf("%S", filePart);
            }

            printf("\n----------------------------------\n");

            // Get a snapshot of all threads and look for the ones
            // from the relevant process
            snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (snapshot != INVALID_HANDLE_VALUE)
            {
                THREADENTRY32 thread;
                thread.dwSize = sizeof(thread);

                // Walk the thread list and print each wait chain
                if (Thread32First(snapshot, &thread))
                {
                    do
                    {
                        if (thread.th32OwnerProcessID == processes[index])
                        {
                            // Open a handle to this specific thread
                            HANDLE thread_handle = OpenThread(THREAD_ALL_ACCESS, FALSE, thread.th32ThreadID);
                            if (thread_handle)
                            {
                                // Check whether the thread is still running
                                DWORD exit_code;
                                GetExitCodeThread(thread_handle, &exit_code);

                                if (exit_code == STILL_ACTIVE)
                                {
                                    // Print the wait chain.
                                    PrintWaitChain(thread.th32ThreadID);
                                }

                                CloseHandle(thread_handle);
                            }
                        }
                    } while (Thread32Next(snapshot, &thread));
                }
                CloseHandle(snapshot);
            }

            CloseHandle(process);
            printf("\n");
        }
    }
    return TRUE;
}

/*++

Routine Description:

    Enumerates all threads (or optionally only threads for one
    process) in the system. It the calls the WCT API on each thread.

Arguments:

    ThreadId--Specifies the thread ID to analyze.

Return Value:

    (none)

--*/

void PrintWaitChain(__in DWORD thread_id)
{
    WAITCHAIN_NODE_INFO node_info_array[WCT_MAX_NODE_COUNT];
    DWORD               counters, index;
    BOOL                is_cycle;

    printf("%d: ", thread_id);

    counters = WCT_MAX_NODE_COUNT;

    // Make a synchronous WCT call to retrieve the wait chain.
    if (!GetThreadWaitChain(g_WctHandle, NULL, WCTP_GETINFO_ALL_FLAGS, thread_id, &counters, node_info_array, &is_cycle))
    {
        printf("Error (0X%x)\n", GetLastError());
        return;
    }

    // Check if the wait chain is too big for the array we passed in.
    if (counters > WCT_MAX_NODE_COUNT)
    {
        printf("Found additional nodes %d\n", counters);
        counters = WCT_MAX_NODE_COUNT;
    }

    // Loop over all the nodes returned and print useful information.
    for (index = 0; index < counters; index++)
    {
        switch (node_info_array[index].ObjectType)
        {
        case WctThreadType:
            // A thread node contains process and thread ID.
            printf("[%x:%x:%s]->",
                node_info_array[index].ThreadObject.ProcessId,
                node_info_array[index].ThreadObject.ThreadId,
                ((node_info_array[index].ObjectStatus == WctStatusBlocked) ? "b" : "r"));
            break;

        default:
            // A synchronization object.

            // Some objects have names...
            if (node_info_array[index].LockObject.ObjectName[0] != L'\0')
            {
                printf("[%s:%S]->",
                    STR_OBJECT_TYPE[node_info_array[index].ObjectType - 1].m_description,
                    node_info_array[index].LockObject.ObjectName);
            }
            else
            {
                printf("[%s]->",
                    STR_OBJECT_TYPE[node_info_array[index].ObjectType - 1].m_description);
            }
            if (node_info_array[index].ObjectStatus == WctStatusAbandoned)
            {
                printf("<abandoned>");
            }
            break;
        }
    }

    printf("[End]");

    // Did we find a deadlock?
    if (is_cycle)
    {
        printf(" !!!Deadlock!!!");
    }

    printf("\n");
}

/*++

Routine Description:

  Print usage information to stdout.

--*/

void Usage()
{
    printf("\nPrints the thread wait chains for one or all processes in the system.\n\n");
    printf("\nUsage:\tWctEnum [ProcId]\n");
    printf("\t (no params) -- get the wait chains for all processes\n");
    printf("\t ProcId      -- get the wait chains for the specified process\n\n");
}

/*++

Routine Description:

    Register COM interfaces with WCT. This enables WCT to provide wait
    information if a thread is blocked on a COM call.

--*/

BOOL InitCOMAccess()
{
    PCOGETCALLSTATE       call_state_callback;
    PCOGETACTIVATIONSTATE activation_state_callback;

    // Get a handle to OLE32.DLL. You must keep this handle around
    // for the life time for any WCT session.
    g_Ole32Hnd = LoadLibrary(L"ole32.dll");
    if (!g_Ole32Hnd)
    {
        printf("ERROR: GetModuleHandle failed: 0x%X\n", GetLastError());
        return FALSE;
    }

    // Retrieve the function addresses for the COM helper APIs.
    call_state_callback = (PCOGETCALLSTATE) GetProcAddress(g_Ole32Hnd, "CoGetCallState");

    if (!call_state_callback)
    {
        printf("ERROR: GetProcAddress failed: 0x%X\n", GetLastError());
        return FALSE;
    }

    activation_state_callback = (PCOGETACTIVATIONSTATE) GetProcAddress(g_Ole32Hnd, "CoGetActivationState");
    if (!activation_state_callback)
    {
        printf("ERROR: GetProcAddress failed: 0x%X\n", GetLastError());
        return FALSE;
    }

    // Register these functions with WCT.
    RegisterWaitChainCOMCallback(call_state_callback, activation_state_callback);

    return TRUE;
}

int _cdecl wmain(__in int argc, __in_ecount(argc) PWSTR* argv)
{
    int rc = 1;

    // Initialize the WCT interface to COM. Continue if this
    // fails--there just will not be COM information.
    if (!InitCOMAccess())
    {
        printf("Could not enable COM access\n");
    }

    // Open a synchronous WCT session.
    g_WctHandle = OpenThreadWaitChainSession(0, NULL);
    if (NULL == g_WctHandle)
    {
        printf("ERROR: OpenThreadWaitChainSession failed\n");
        goto Cleanup;
    }

    if (argc < 2)
    {
        // Enumerate all threads in the system.
        CheckThreads(0);
    }
    else
    {
        // Only enumerate threads in the specified process.
        //
        // Take the first command line parameter as the process ID.
        DWORD  process_id = 0;

        process_id = _wtoi(argv[1]);
        if (process_id == 0)
        {
            Usage();
            goto Cleanup;
        }

        CheckThreads(process_id);
    }

    // Close the WCT session.
    CloseThreadWaitChainSession(g_WctHandle);

    rc = 0;

Cleanup:

    if (NULL != g_Ole32Hnd)
    {
        FreeLibrary(g_Ole32Hnd);
    }

    return rc;
}