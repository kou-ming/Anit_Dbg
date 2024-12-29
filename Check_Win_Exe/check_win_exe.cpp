#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

using namespace std;


BOOL checkDbgWindow() {
    HWND hwnd = FindWindow(NULL, L"x32dbg");
    if (hwnd) {
        printf("檢測到x32dbg的視窗!\n");
        return TRUE;
    }
    return FALSE;
}

BOOL checkDbgProcess() {
    PROCESSENTRY32W processEntry{ 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32W);
    HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (!handle) {
        return FALSE;
    }
    Process32FirstW(handle, &processEntry);
    do {
        if (wcscmp(L"x32dbg.exe", processEntry.szExeFile) == 0) {
            CloseHandle(handle);
            printf("檢測到x32dbg.exe!\n");
            return TRUE;
        }
    } while (Process32NextW(handle, &processEntry));
    CloseHandle(handle);
    return FALSE;
}

DWORD WINAPI checkDbg(LPVOID lparam) {
    while (true) {
        if (checkDbgWindow() || checkDbgProcess()) {
            printf("now debugged!!!\n");
            break;
        }
    }
    return 0;
}

int main() {
    cout << "Start!" << endl;
    HANDLE hthread = CreateThread(NULL, 0, checkDbg, 0, 0, NULL);
    while (true) {
        printf("haha haha!\n");
        Sleep(2000);
    }
    return 0;
}