#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>

int main()
{
	const char* source_path = "input.txt";
	const char* dest_path_prefix = "output";
	const char* dest_path_suffix = ".txt";

	HANDLE handle_input_file = CreateFile(source_path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

	if (handle_input_file != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER li;
		GetFileSizeEx(handle_input_file, &li);
		const __int64 main_file_size = li.QuadPart;
		const __int64 split_file_size = 1024 * 1024;

		static const std::size_t chunk_size = 1024;
		
		void* buffer = VirtualAlloc(0, chunk_size, MEM_COMMIT, PAGE_READWRITE);

		for (__int64 position = 0; position < main_file_size; position += split_file_size)
		{
			std::string dest_path;
			{
				static int cnt = 0;
				std::ostringstream stm;
				stm << dest_path_prefix << std::setw(3) << std::setfill('0') << cnt++ << dest_path_suffix;
				dest_path = stm.str();
			}
			HANDLE outfile = CreateFile(dest_path.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, handle_input_file);
			if (outfile != INVALID_HANDLE_VALUE)
			{
				// read and write CHUNKSZ bytes at a time
				for (__int64 n = 0; n < split_file_size; n += chunk_size)
				{
					DWORD read;
					ReadFile(handle_input_file, buffer, chunk_size, &read, 0);
					if (read)
					{
						WriteFile(outfile, buffer, read, 0, 0);
					}
					if (read < chunk_size)
					{
						break; // last chunk
					}
				}
				CloseHandle(outfile);
			}
		}

		CloseHandle(handle_input_file);
		VirtualFree(buffer, 0, MEM_RELEASE);
	}
}