#include<iostream>
#include <windows.h>

using namespace std;

#define FLG_HEAP_ENABLE_TAIL_CHECK   0x10
#define FLG_HEAP_ENABLE_FREE_CHECK   0x20
#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
#define NT_GLOBAL_FLAG_DEBUGGED (FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS)

PVOID GetPEB()
{
#ifdef _WIN64
    return (PVOID)__readgsqword(0x0C * sizeof(PVOID));
#else
    return (PVOID)__readfsdword(0x0C * sizeof(PVOID));
#endif
}

void CheckNtGlobalFlag()
{
    PVOID pPeb = GetPEB();
    DWORD offsetNtGlobalFlag = 0;
#ifdef _WIN64
    cout << "_WIN64" << endl;
    offsetNtGlobalFlag = 0xBC;
#else
    cout << "_WIN32" << endl;
    offsetNtGlobalFlag = 0x68;
#endif
    DWORD NtGlobalFlag = *(PDWORD)((PBYTE)pPeb + offsetNtGlobalFlag);
    cout << "NtGlobalFlag is: " << NtGlobalFlag << endl;
    if (NtGlobalFlag & NT_GLOBAL_FLAG_DEBUGGED)
    {
        std::cout << "Stop debugging program!" << std::endl;
        exit(-1);
    }
    else {
        std::cout << "NO debugging now" << std::endl;
    }
}

int main() {
    cout << "start" << endl;
    CheckNtGlobalFlag();
    return 0;
}