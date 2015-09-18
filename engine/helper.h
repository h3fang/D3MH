#ifndef HELPER
#define HELPER

#include <windows.h>

bool GetProcessList();
HANDLE GetProcessByName(const wchar_t *name);
bool SetPrivilege( HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
bool AdjustDebugPrivilege();

#endif // HELPER

