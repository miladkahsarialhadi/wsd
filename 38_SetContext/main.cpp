#include <Windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <process.h>

static CONTEXT gContext = { 0 };

void ShowThreadContext(CONTEXT arg_context)
{
	printf("\tEax : %#010x\n", arg_context.Eax);
	printf("\tEbx : %#010x\n", arg_context.Ebx);
	printf("\tEcx : %#010x\n", arg_context.Ecx);
	printf("\tEdx : %#010x\n", arg_context.Edx);
	printf("\tEsi : %#010x\n", arg_context.Esi);
	printf("\tEdi : %#010x\n", arg_context.Edi);
	printf("\tEbp : %#010x\n", arg_context.Ebp);
	printf("\tEip : %#010x\n", arg_context.Eip);
	printf("\tEsp : %#010x\n", arg_context.Esp);
	printf("\tCS : %#010x\n", arg_context.SegCs);
	printf("\tDS : %#010x\n", arg_context.SegDs);
	printf("\tES : %#010x\n", arg_context.SegEs);
	printf("\tFS : %#010x\n", arg_context.SegFs);
	printf("\tGS : %#010x\n", arg_context.SegGs);
	printf("\tSS : %#010x\n", arg_context.SegSs);
}

unsigned __stdcall PrintMessageMultiple(void* arg_param)
{
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "Hello from " << __FUNCTION__ << std::endl;
	}

	return 0;
}

unsigned __stdcall PrintMessageSingle(void* arg_param)
{
	std::cout << "Hello from " << __FUNCTION__ << std::endl;

	return 0;
}

int main(int argc, char* argv[])
{
	std::cout << "Primary Thread Execution Context:" << std::endl;
	gContext.ContextFlags = CONTEXT_ALL;
	GetThreadContext(GetCurrentThread(), &gContext);
	ShowThreadContext(gContext);
	
	std::cout << "Second Thread Execution Context:" << std::endl;
	HANDLE thread_second = (HANDLE)_beginthreadex(NULL, 0, PrintMessageMultiple, NULL, 0, NULL);
	GetThreadContext(thread_second, &gContext);
	ShowThreadContext(gContext);
	WaitForSingleObject(thread_second, INFINITE);

	std::cout << "Third Thread Execution Context:" << std::endl;
	HANDLE thread_third = (HANDLE)_beginthreadex(NULL, 0, PrintMessageSingle, NULL, 0, NULL);
	SetThreadContext(thread_third, &gContext);
	ShowThreadContext(gContext);
	WaitForSingleObject(thread_third, INFINITE);

	CloseHandle(thread_second);
	CloseHandle(thread_third);

	return 0;
}