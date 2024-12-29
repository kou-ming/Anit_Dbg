#include <windows.h>
#include <iostream>

using namespace std;

typedef NTSTATUS (NTAPI *pfnNtSetInformationThread)(
    _In_ HANDLE ThreadHandle,
    _In_ ULONG ThreadInformationClass,
    _In_ PVOID ThreadInformation,
    _In_ ULONG ThreadInformationLength
);

const ULONG ThreadHideFromDebugger = 0x11;
void HideFromDebugger(){
    HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
    pfnNtSetInformationThread NtSetInformationThread = (pfnNtSetInformationThread)GetProcAddress(hNtDll, "NtSetInformationThread");

    NTSTATUS status = NtSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, NULL, 0);
}

int main() {
    cout << "Start" << endl;
    HideFromDebugger();
    cout << "End~" << endl;
    system("pause");
    return 0;
}