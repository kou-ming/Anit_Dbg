#include <windows.h>
#include "mhook-lib/mhook.h"

typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
    _In_      HANDLE           ProcessHandle,
    _In_      UINT             ProcessInformationClass,
    _Out_     PVOID            ProcessInformation,
    _In_      ULONG            ProcessInformationLength,
    _Out_opt_ PULONG           ReturnLength    
);
const UINT ProcessDebugPort = 7;
pfnNtQueryInformationProcess g_origNtQueryInformationProcess = NULL;
NTSTATUS NTAPI HookNtQueryInformationProcess(
    _In_      HANDLE           ProcessHandle,
    _In_      UINT             ProcessInformationClass,
    _Out_     PVOID            ProcessInformation,
    _In_      ULONG            ProcessInformationLength,
    _Out_opt_ PULONG           ReturnLength    
){
    NTSTATUS status = g_origNtQueryInformationProcess(
        ProcessHandle,
        ProcessInformationClass,
        ProcessInformation,
        ProcessInformationLength,
        ReturnLength);
    if(status == 0x00000000 && ProcessInformationClass == ProcessDebugPort){
        *((PDWORD_PTR)ProcessInformation) = 0;
    }
    return 0;
}

DWORD SetupHook(PVOID pvContext){
    HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
    if(hNtDll != NULL){
        g_origNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
        if(g_origNtQueryInformationProcess != NULL){
            Mhook_SetHook((PVOID*)&g_origNtQueryInformationProcess, HookNtQueryInformationProcess);
        }
    }
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved){
    switch(fdwReason){
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupHook, NULL, NULL, NULL);
            Sleep(20);
        case DLL_PROCESS_DETACH:
            if(g_origNtQueryInformationProcess != NULL){
                Mhook_Unhook((PVOID*)&g_origNtQueryInformationProcess);
            }
            break;
    }
    return true;
}
