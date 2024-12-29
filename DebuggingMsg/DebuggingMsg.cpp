#include <iostream>
#include <windows.h>

using namespace std;

#define DBG_PRINTEXCEPTION_WIDE_C 0x4001000A
WCHAR* outputString = L"Any text";
ULONG_PTR args[4] = {0};


int main(){
    cout << "Start" << endl;

    args[0] = (ULONG_PTR)wcslen(outputString) + 1;
    args[1] = (ULONG_PTR)outputString;

    __try{
        RaiseException(DBG_PRINTEXCEPTION_WIDE_C, 0, 4, args);
        cout << "Debugger detected" << endl;
        system("pause");
        exit(-1);
    }
    __except(EXCEPTION_EXECUTE_HANDLER){
        cout << "Debugger Not Detected" << endl;
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}