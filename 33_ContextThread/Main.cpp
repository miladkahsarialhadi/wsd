#include <tchar.h>
#include <Windows.h>
#include <Shlwapi.h>

void ShowThreadContext(CONTEXT arg_context)
{
	_tprintf(TEXT("\tEax : %#010x\n"), arg_context.Eax);
	_tprintf(TEXT("\tEbx : %#010x\n"), arg_context.Ebx);
	_tprintf(TEXT("\tEcx : %#010x\n"), arg_context.Ecx);
	_tprintf(TEXT("\tEdx : %#010x\n"), arg_context.Edx);
	_tprintf(TEXT("\tEsi : %#010x\n"), arg_context.Esi);
	_tprintf(TEXT("\tEdi : %#010x\n"), arg_context.Edi);
	_tprintf(TEXT("\tEbp : %#010x\n"), arg_context.Ebp);
	_tprintf(TEXT("\tEip : %#010x\n"), arg_context.Eip);
	_tprintf(TEXT("\tEsp : %#010x\n"), arg_context.Esp);
	_tprintf(TEXT("\tCS : %#010x\n"), arg_context.SegCs);
	_tprintf(TEXT("\tDS : %#010x\n"), arg_context.SegDs);
	_tprintf(TEXT("\tES : %#010x\n"), arg_context.SegEs);
	_tprintf(TEXT("\tFS : %#010x\n"), arg_context.SegFs);
	_tprintf(TEXT("\tGS : %#010x\n"), arg_context.SegGs);
	_tprintf(TEXT("\tSS : %#010x\n"), arg_context.SegSs);
}

DWORD WINAPI ContextShow(LPVOID arg_param)
{
	CONTEXT context;
	ZeroMemory(&context, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(GetCurrentThread(), &context);
	ShowThreadContext(context);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	
	HANDLE handle_thread = CreateThread(NULL, 10, ContextShow, NULL, THREAD_GET_CONTEXT | CREATE_SUSPENDED, NULL);
	if (handle_thread == INVALID_HANDLE_VALUE || handle_thread == NULL) 
	{
		_tprintf(TEXT("error to create thread\n"));
		return 1;
	}
	else 
	{
		_tprintf(TEXT("Primary Thread Context: \n"));
		CONTEXT context;
		ZeroMemory(&context, sizeof(CONTEXT));
		context.ContextFlags = CONTEXT_ALL;
		GetThreadContext(GetCurrentThread(), &context);
		ShowThreadContext(context);
		
		_tprintf(TEXT("\nSecondary Thread Context: \n"));
		ResumeThread(handle_thread);
		WaitForSingleObject(handle_thread, INFINITE);
	}

	CloseHandle(handle_thread);

	return 0;
}