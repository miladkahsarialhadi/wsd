#include <windows.h>
#include <iostream>

#define CACHE_ALIGN 64

struct __declspec(align(CACHE_ALIGN)) ALIGNED
{
	// Force each structure to be in a different cache line.
	DWORD m_CustomerId;		// Mostly read-only
	wchar_t m_Name[100];	// Mostly read-only

	// Force the following members to be in a different cache line.
	alignas(CACHE_ALIGN)
	int m_Balance;				// Read-write
	FILETIME m_LastOrderDate;	// Read-write
};

struct NOTALIGNED
{
	DWORD m_CustomerId;		// Mostly read-only
	wchar_t m_Name[100];	// Mostly read-only

	int m_Balance;				// Read-write
	FILETIME m_LastOrderDate;	// Read-write
};

int _cdecl main(int argc, const char* argv[])
{
	volatile ALIGNED	O1;
	volatile NOTALIGNED	O2;

	std::cout << "Size of aligned struct? " << sizeof(O1) << std::endl;
	std::cout << "Size of not aligned struct? " << sizeof(O2) << std::endl;

	return 0;
}
