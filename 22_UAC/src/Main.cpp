#include <windows.h>
#include <Shlobj.h>
#include <iostream>

TOKEN_ELEVATION_TYPE g_tet_ElevationType = TokenElevationTypeDefault;
BOOL                 g_b_IsAdmin = FALSE;

BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE* arg_elevation_type, BOOL* arg_is_admin) {
	HANDLE h_Token = NULL;
	DWORD dw_Size;

	// Get current process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_Token))
	{
		return FALSE;
	}


	BOOL b_Result = FALSE;
	// Retrieve elevation type information
	if (GetTokenInformation(h_Token, TokenElevationType, arg_elevation_type, sizeof(TOKEN_ELEVATION_TYPE), &dw_Size)) {

		// Create the SID corresponding to the Administrators group
		BYTE l_bt_AdminSID[SECURITY_MAX_SID_SIZE];
		dw_Size = sizeof(l_bt_AdminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &l_bt_AdminSID, &dw_Size);

		if (*arg_elevation_type == TokenElevationTypeLimited) {

			// Get handle to linked token (will have one if we are lua)
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(h_Token, TokenLinkedToken, (VOID*)&hUnfilteredToken, sizeof(HANDLE), &dw_Size);

			// Check if this original token contains admin SID
			if (CheckTokenMembership(hUnfilteredToken, &l_bt_AdminSID, arg_is_admin)) {
				b_Result = TRUE;
			}

			// Don't forget to close the unfiltered token
			CloseHandle(hUnfilteredToken);
		}
		else {
			*arg_is_admin = IsUserAnAdmin();
			b_Result = TRUE;
		}
	}

	// Don't forget to close the process token
	CloseHandle(h_Token);
	return b_Result;
}


// returns 0 if an Admin and/or elevated
// returns 1 if a filtered user token (Admin but UAC'd)
// returns 2 if a regular user
// returns 5 if none of the standard elevation types match
// returns 10 if GetProcessElevation failed
int GetUserTokenStatus()
{
	int dw_UserTokenStatus = 10;
	if (GetProcessElevation(&g_tet_ElevationType, &g_b_IsAdmin)) {
		switch (g_tet_ElevationType) {
		// Default user or UAC is disabled
		case TokenElevationTypeDefault:
			if (IsUserAnAdmin()) {
				dw_UserTokenStatus = 0;
			}
			else {
				dw_UserTokenStatus = 2;
			}
			break;
		
		// Process has been successfully elevated
		case TokenElevationTypeFull:
			if (IsUserAnAdmin()) {
				dw_UserTokenStatus = 0;
			}
			else {
				dw_UserTokenStatus = 0;
			}
			break;
		
		// Process is running with limited privileges
		case TokenElevationTypeLimited:
			if (IsUserAnAdmin()) {
				// Not sure what capabilities a filtered Admin has
				// So, for now, I'll treat that as non-Admin
				dw_UserTokenStatus = 3;
			}
			else {
				dw_UserTokenStatus = 1;
			}
			break;

		default:
			dw_UserTokenStatus = 5;
		}
	}

	return dw_UserTokenStatus;
}

int main(int argc, const char* argv[])
{
	if (GetUserTokenStatus() == 0)
	{
		std::cout << "I am Administrator." << std::endl;
	}
	else 
	{
		std::cout << "I am Standard User." << std::endl;
	}

	std::cin.get();
	return 0;
}