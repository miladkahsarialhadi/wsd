#include <Windows.h>
#include <stdio.h>

void PasswordCheck() {
	char message_console[MAX_PATH];
	char key_accepted[MAX_PATH];
	char bad_key[MAX_PATH];
	char password_store[MAX_PATH];
	DWORD chars_read;

	strcpy(message_console, "Enter password for this malware: ");
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message_console, strlen(message_console), NULL, NULL);
	ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), password_store, MAX_PATH, &chars_read, NULL);

	if (!strcmp(password_store, "$mab")) 
	{
		strcpy(key_accepted, "Key Accepted");
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), key_accepted, strlen(key_accepted), NULL, NULL);
	}
	else 
	{
		strcpy(bad_key, "Bad key");
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), bad_key, strlen(bad_key), NULL, NULL);
	}
}

int main(int argc, char* argv[]) {
	PasswordCheck();
	return 0;
}