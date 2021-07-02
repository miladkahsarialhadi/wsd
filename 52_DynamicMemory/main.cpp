#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <vector>

namespace mka
{
	namespace memory
	{
		VOID allocation()
		{
			auto string = static_cast<PCHAR>(LocalAlloc(LMEM_FIXED, 6 + 1));
			if (!string) return;

			std::strcpy(string, "WINAPI");
			std::printf("%s\n", string);

			PCHAR newstring = static_cast<char*>(LocalReAlloc(string, 6 + 12 + 1, LMEM_MOVEABLE));
			if (!newstring) 
			{ 
				LocalFree(string); 
				return; 
			}
			string = newstring;

			std::strcpy(string + 6, " IS THE BEST");
			std::printf("%s\n", string);

			LocalFree(string);
		}
	}

	namespace parser
	{
		struct INFORMATION
		{
			DWORD m_id;
			PCHAR m_name;
		};

		VOID sequence()
		{
			DWORD index = 0;
			std::vector<std::string> names = { "KoreKhar", "KoreAsb", "ShotorMorgh" };

			auto o_information = static_cast<INFORMATION*>(LocalAlloc(LPTR, sizeof(INFORMATION) * 3));
			for (index = 0; index < 3; ++index)
			{
				o_information[index].m_id = index;
				DWORD name_size = lstrlenA(names[index].c_str());
				o_information[index].m_name = static_cast<PCHAR>(LocalAlloc(LPTR, name_size + 1));
				if (o_information[index].m_name != NULL)
				{
					ZeroMemory(o_information[index].m_name, name_size);
					CopyMemory(o_information[index].m_name, names[index].c_str(), name_size);
					o_information[index].m_name[name_size] = 0;
				}
			}

			for (index = 0; index < 3; ++index)
			{
				CHAR output[512];
				wsprintfA(output, "#%d %s", o_information[index].m_id, o_information[index].m_name);
				MessageBoxA(NULL, output, "Output", MB_OKCANCEL);
			}
		}
	}

	namespace database
	{
		DWORD user_id = 0;

		struct DATABASE
		{
			DWORD m_id;
			PCHAR m_name;
			PCHAR m_password;

			DATABASE* m_next;
		}*TABLE;

		VOID add_record(DATABASE* arg_table, PCHAR arg_username, PCHAR arg_password)
		{
			auto node = static_cast<DATABASE*>(LocalAlloc(LPTR, sizeof(DATABASE)));
			if (node != ERROR)
			{
				while (arg_table->m_next != NULL)
				{
					// Keep going until you reach to null member
					// then we can add new record to that
					arg_table = arg_table->m_next;
				}
				node->m_id = user_id++;
				node->m_name = arg_username;
				node->m_password = arg_password;

				arg_table->m_next = node;
			}
		}

		DATABASE* search_records(DATABASE* arg_table, DWORD arg_id)
		{
			while (arg_table->m_next != NULL)
			{
				if (arg_table->m_next->m_id == arg_id)
				{
					return arg_table->m_next;
				}
				arg_table = arg_table->m_next;
			}
		}

		VOID remove(DATABASE* arg_table, DWORD arg_id)
		{
			while (arg_table->m_next != NULL)
			{
				if (arg_table->m_id == arg_id)
				{
					arg_table->m_next = static_cast<DATABASE*>(LocalAlloc(LPTR, sizeof(DATABASE)));
					if (arg_table->m_next != NULL)
					{
						auto element = arg_table->m_next;

						arg_table->m_id = arg_table->m_next->m_id;
						arg_table->m_name = arg_table->m_next->m_name;
						arg_table->m_password = arg_table->m_next->m_password;
						arg_table->m_next = element->m_next;

						LocalFree(element);
					}
				}
				arg_table = arg_table->m_next;
			}
		}
		PCHAR users[3][2] = {
			{"Milad", "123"},
			{"Rolf", "321"},
			{"HD", "213"}
		};

		PCHAR allocated_buffer(PCHAR arg_buffer, DWORD arg_length)
		{
			PCHAR out = static_cast<PCHAR>(LocalAlloc(LPTR, arg_length + 1));
			if (out != ERROR)
			{
				CopyMemory(out, arg_buffer, arg_length);
				out[arg_length] = 0;
				return out;
			}
			return ERROR;
		}
	}
}

int main(int argc, char* argv[])
{
	//mka::database::DATABASE* table = static_cast<mka::database::DATABASE*>(LocalAlloc(LPTR, sizeof(mka::database::DATABASE)));
	auto table = static_cast<mka::database::DATABASE*>(LocalAlloc(LPTR, sizeof(mka::database::DATABASE)));

	mka::database::DATABASE* element;

	if (table != ERROR)
	{
		for (DWORD index = 0; index < 3; ++index)
		{
			auto username = mka::database::allocated_buffer(mka::database::users[index][0], lstrlenA(mka::database::users[index][0]));
			auto password = mka::database::allocated_buffer(mka::database::users[index][1], 3);
			mka::database::add_record(table, username, password);
		}
	}

	element = mka::database::search_records(table, 1);
	std::printf("\tName: %s\n\tID:%d", element->m_name, element->m_id);

	return 0;
}