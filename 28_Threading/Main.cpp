#include <windows.h>
#include <iostream>
#include <vector>
#include <tchar.h>

int _tmain(int argc, wchar_t* argv[])
{
	std::vector<std::wstring> argument_list;
	argument_list.assign(argv + 1, argv + argc);

	std::wcout << "First: " << argument_list[0] << std::endl;
	
	CONTEXT context;
	ZeroMemory(&context, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(GetCurrentThread(), &context);
	std::printf("EFLAGS: \n");
	std::printf("\tDr0 : %#010x\n", context.Dr0);
	std::printf("\tDr1 : %#010x\n", context.Dr1);
	std::printf("\tDr2 : %#010x\n", context.Dr2);
	std::printf("\tDr3 : %#010x\n", context.Dr3);
	std::printf("\tDr4 : %#010x\n", context.Dr6);
	std::printf("\tDr7 : %#010x\n", context.Dr7);
	std::printf("\tEax : %#010x\n", context.Eax);
	std::printf("\tEbx : %#010x\n", context.Ebx);
	std::printf("\tEcx : %#010x\n", context.Ecx);
	std::printf("\tEdx : %#010x\n", context.Edx);
	std::printf("\tEsi : %#010x\n", context.Esi);
	std::printf("\tEdi : %#010x\n", context.Edi);
	std::printf("\tEbp : %#010x\n", context.Ebp);
	std::printf("\tEip : %#010x\n", context.Eip);
	std::printf("\tEsp : %#010x\n", context.Esp);
	std::printf("\tSegCs : %#010x\n", context.SegCs);
	std::printf("\tSegDs : %#010x\n", context.SegDs);
	std::printf("\tSegEs : %#010x\n", context.SegEs);
	std::printf("\tSegFs : %#010x\n", context.SegFs);
	std::printf("\tSegGs : %#010x\n", context.SegGs);
	std::printf("\tSegSs : %#010x\n", context.SegSs);

	return 0;
}