#ifndef HELPER
#define HELPER

#include <windows.h>

bool GetProcessList();
DWORD GetProcessIdByName(const wchar_t *name);
DWORD GetProcessBaseAddress(DWORD processId, const wchar_t *name);
bool SetPrivilege( HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
bool AdjustDebugPrivilege();

#endif // HELPER

