#include <Windows.h>
#include <iostream>

using namespace std;



int main() {
    cout << "Start!" << endl;
    
    SC_HANDLE hSchandle = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSchandle == NULL) {
        printf("OpenSCManagerA failed with error: %lu\n", GetLastError());
        system("pause");
        return 0;
    }

    LPENUM_SERVICE_STATUSA lpService = NULL;
    DWORD bufferSize = 0x4000;
    DWORD realSize = 0;
    DWORD serviceNumber = 0;
    DWORD resumeHandle = 0;
    BOOL ret;
    do {
        lpService = (LPENUM_SERVICE_STATUSA)malloc(bufferSize);
        ret = EnumServicesStatusA(hSchandle, SERVICE_WIN32, SERVICE_STATE_ALL, lpService,
            0x4000, &realSize, &serviceNumber, &resumeHandle);
        if (!ret) {
            DWORD error = GetLastError();
            if (error == ERROR_MORE_DATA) {
                free(lpService);
                lpService = NULL;
                bufferSize = realSize;
            }
            else {
                printf("EnumServicesStatusA failed with error: %lu\n", GetLastError());
                free(lpService);
                CloseServiceHandle(hSchandle);
                system("pause");
                return  0;
            }
        }
    } while (!ret);

    printf("Number of services: %lu\n", serviceNumber);
    for (size_t i = 0; i < serviceNumber; i++) {
        if (strstr(lpService->lpDisplayName, "VMware") == 0) {
            MessageBoxA(0, "檢測到虛擬機", "提示", MB_OK);
            break;
        }
        //cout << lpService->lpDisplayName << endl;
    }
    //cout << "no Debugger" << endl;
    system("pause");
    free(lpService);
    CloseServiceHandle(hSchandle);
    return 0;
}