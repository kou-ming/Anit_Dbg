#include <windows.h>
#include <winternl.h>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
    _In_      HANDLE           ProcessHandle,
    _In_      UINT             ProcessInformationClass,
    _Out_     PVOID            ProcessInformation,
    _In_      ULONG            ProcessInformationLength,
    _Out_opt_ PULONG           ReturnLength
);

typedef struct _PROCESS_BASIC_INFORMATION_CUSTOM {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION_CUSTOM;

wstring GetProcessNameById(DWORD pid){
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE){
        return 0;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    wstring processName = L"";
    if(!Process32First(hProcessSnap, &pe32)){
        CloseHandle(hProcessSnap);
        return processName;
    }
    do{
        if(pe32.th32ProcessID == pid){
            processName = wstring(pe32.szExeFile, pe32.szExeFile + strlen(pe32.szExeFile));
            break;
        }
    }
    while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return processName;
}

const UINT ProcessBasicInf = 0x00;
int main(int argc, char *argv[])
{
    cout << "Start!!!" << endl;
    pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
    NTSTATUS status;
    PROCESS_BASIC_INFORMATION_CUSTOM processBasicInformation;
    HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
    
    if (NULL != hNtDll)
    {
        NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
        if (NULL != NtQueryInformationProcess)
        {
            status = NtQueryInformationProcess(
                GetCurrentProcess(),
                ProcessBasicInf,
                &processBasicInformation,
                sizeof(PROCESS_BASIC_INFORMATION_CUSTOM),
                NULL);
            wstring parentProcessName = GetProcessNameById((DWORD)processBasicInformation.InheritedFromUniqueProcessId);
            wcout << parentProcessName << endl;
        }
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}