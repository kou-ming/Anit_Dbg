#include <windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;


void DebugLoop() {
    DEBUG_EVENT DebugEvent;
    while (1) {
        // 等待調試事件
        if (WaitForDebugEvent(&DebugEvent, INFINITE)) {
            // 檢查事件類型
            if (DebugEvent.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) {
                LOAD_DLL_DEBUG_INFO &DllInfo = DebugEvent.u.LoadDll;

                // printf("DLL Loaded at Base Address: %p\n", DllInfo.lpBaseOfDll);

                // 可進一步使用 hFile 或 lpImageName 取得更多信息
                // ...

                // 關閉 DLL 文件句柄（如果已開啟）
                if (DllInfo.hFile) {
                    CloseHandle(DllInfo.hFile);
                }
            }

            // 繼續調試
            ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, DBG_CONTINUE);
        }
    }
}

int main(int argc, char *argv[]) {
    cout << argv[1] <<endl;
    if(argc < 2){
        cout << "[-] Need a argument !" << endl;
    }
    else{
        //例：啟動要調試的進程
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (CreateProcess(argv[1], NULL, NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL, &si, &pi)) {
            DebugLoop();
        } else {
            printf("Failed to start process.\n");
        }
    }
    return 0;
}