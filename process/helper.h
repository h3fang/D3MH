#ifndef UTILITIES_HELPER
#define UTILITIES_HELPER

#include <windows.h>

struct HandleData {
    DWORD process_id;
    HWND best_handle;
};

HWND FindMainWindow(DWORD process_id);
BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
BOOL IsMainWindow(HWND handle);

bool GetProcessList();
DWORD GetProcessIdByName(const wchar_t *name);
DWORD GetProcessBaseAddress(DWORD processId, const wchar_t *name);
bool SetPrivilege( HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
bool AdjustDebugPrivilege();

#endif // UTILITIES_HELPER

