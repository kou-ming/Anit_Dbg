#include <Windows.h>
#include <iostream>

using namespace std;

typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
    _In_      HANDLE           ProcessHandle,
    _In_      UINT             ProcessInformationClass,
    _Out_     PVOID            ProcessInformation,
    _In_      ULONG            ProcessInformationLength,
    _Out_opt_ PULONG           ReturnLength
);
const UINT ProcessDebugObjectHandle = 0x1E;
int main(int argc, char *argv[])
{
    cout << "Start!!!" << endl;
    pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
    NTSTATUS status;
    HANDLE hProcessDebugObject = NULL;
    HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
    
    if (NULL != hNtDll)
    {
        NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
        if (NULL != NtQueryInformationProcess)
        {
            status = NtQueryInformationProcess(
                GetCurrentProcess(),
                ProcessDebugObjectHandle,
                &hProcessDebugObject,
                sizeof(HANDLE),
                NULL);
            if (status == 0x00000000 && hProcessDebugObject != NULL)
            {
                cout << "Stop debugging program!" << endl;
                system("pause");
                exit(-1);
            }
        }
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}