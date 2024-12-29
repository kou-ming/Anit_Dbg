// 要使用x86的MSCV編譯__asm
#include <iostream>
#include <windows.h>

using namespace std;

BOOL isDebugged = true;

int main(){
    cout << "Start!!!" << endl;
    __try{
        cout << "test1" <<endl;
        __asm{
            pushfd
            or dword ptr[esp], 0x100
            popfd
            nop
        }
        cout << "test2" <<endl;
    }
    __except (EXCEPTION_EXECUTE_HANDLER){
        isDebugged = false;
    }
    if(isDebugged){
        cout << "Stop debugging program!!!" << endl;
        system("pause");
        exit(-1);
    }

    cout << "End~" << endl;
    system("pause");
    return 0;
}