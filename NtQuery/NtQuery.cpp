#include <Windows.h>
#include <iostream>

using namespace std;

/*NTATATUS表示函數的返回類型，若成功則返回STATUS_SUCCESS(0x00000000)
NTAPI是一個宏，定義了函數的調用約定()
NTAPI 通常等價於__stdcall
__stdcall指定參數由右到左壓入stack，由被調試者負責清理stack
因為NtQueryInformationProcess不直接暴露在Windows SDK中，通常需要動態加載它
*/
typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
    _In_      HANDLE           ProcessHandle,
    _In_      UINT             ProcessInformationClass,
    _Out_     PVOID            ProcessInformation,
    _In_      ULONG            ProcessInformationLength,
    _Out_opt_ PULONG           ReturnLength
);
const UINT ProcessDebugPort = 7;

int main(int argc, char *argv[])
{
    cout << "Start!!!" << endl;
    pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
    NTSTATUS status;
    DWORD isDebuggerPresent = 0;
    HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
    
    if (NULL != hNtDll)
    {
        NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
        if (NULL != NtQueryInformationProcess)
        {
            status = NtQueryInformationProcess(
                GetCurrentProcess(),
                ProcessDebugPort,
                &isDebuggerPresent,
                sizeof(DWORD),
                NULL);
            if (status == 0x00000000 && isDebuggerPresent != 0)
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