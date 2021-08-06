#include <windows.h>
#include <iostream>


int main(int argc, char* argv[])
{
	// Open the file that we want to map.
	HANDLE handle_file = CreateFile(L"c:\\Sample.dat", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Create a file-mapping object for the file.
	HANDLE hFileMapping = CreateFileMapping(handle_file, NULL, PAGE_WRITECOPY, 0, 0, NULL);

	// Map a copy-on-write view of the file; the system will commit
	// enough physical storage from the paging file to accommodate
	// the entire file. All pages in the view will initially have
	// PAGE_WRITECOPY access.
	PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_COPY, 0, 0, 0);

	// Read a byte from the mapped view.
	BYTE bSomeByte = pbFile[0];
	// When reading, the system does not touch the committed pages in
	// the paging file. The page keeps its PAGE_WRITECOPY attribute.
	// Write a byte to the mapped view.
	pbFile[0] = 0;
	// When writing for the first time, thesystem grabs a committed
// page from the paging file, copies the original contents of the
// page at the accessed memory address, and maps the new page
// (the copy) into the process' address space. The new page has
// an attribute of PAGE_READWRITE.
// Write another byte to the mapped view.
	pbFile[1] = 0;
	// Because this byte is now in a PAGE_READWRITE page, the system
	// simply writes the byte to the page (backed by the paging file).
	// When finished using the file's mapped view, unmap it.
	// UnmapViewOfFile is discussed in the next section.
	UnmapViewOfFile(pbFile);
	// The system decommits the physical storage from the paging file.
	// Any writes to the pages are lost.
	// Clean up after ourselves.
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
	return 0;
}