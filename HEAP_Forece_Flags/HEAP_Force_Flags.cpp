#include <iostream>
#include <windows.h>

using namespace std;

WORD GetVersionWord()
{
    OSVERSIONINFO verInfo = { sizeof(OSVERSIONINFO) };
    GetVersionEx(&verInfo);
    return MAKEWORD(verInfo.dwMinorVersion, verInfo.dwMajorVersion);
}
BOOL IsWin8OrHigher() { return GetVersionWord() >= _WIN32_WINNT_WIN8; }
BOOL IsVistaOrHigher() { return GetVersionWord() >= _WIN32_WINNT_VISTA; }

PVOID GetPEB()
{
#ifdef _WIN64
    return (PVOID)__readgsqword(0x0C * sizeof(PVOID));
#else
    return (PVOID)__readfsdword(0x0C * sizeof(PVOID));
#endif
}

int GetHeapFlagsOffset(bool x64){
    return x64 ? 
        IsVistaOrHigher() ? 0x70 : 0x14:
        IsVistaOrHigher() ? 0x40 : 0x0C;
}

int GetHeapForceFlagsOffset(bool x64){
    return x64 ? 
        IsVistaOrHigher() ? 0x74 : 0x18:
        IsVistaOrHigher() ? 0x44 : 0x10;
}

void CheckHeap(){
    PVOID pPeb = GetPEB();
    PVOID heap = 0;
    DWORD offsetProcessHeap = 0;
    PDWORD heapFlagsPtr = 0, heapForceFlagsPtr = 0;
    BOOL x64 = false;
#ifdef _WIN64
    x64 = true;
    offsetProcessHeap = 0x30;
#else
    offsetProcessHeap = 0x18;
#endif
    heap = (PVOID)*(PDWORD_PTR)((PBYTE)pPeb + offsetProcessHeap);
    heapFlagsPtr = (PDWORD)((PBYTE)heap + GetHeapFlagsOffset(x64));
    heapForceFlagsPtr = (PDWORD)((PBYTE)heap + GetHeapForceFlagsOffset(x64));
    if(*heapFlagsPtr & ~HEAP_GROWABLE || *heapForceFlagsPtr != 0){
        cout << "Stop debugging program!" << endl;
        system("pause");
        exit(-1);
    }
}

int main(){
    cout << "Start!!!" << endl;
    // cout << HEAP_GROWABLE << endl;
    CheckHeap();
    cout << "End~" <<endl;
    system("pause");
    return 0;
}