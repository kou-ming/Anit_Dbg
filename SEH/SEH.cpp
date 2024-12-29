#include <windows.h>
#include <iostream>

using namespace std;

BOOL g_isDebuggerPresent = true;
EXCEPTION_DISPOSITION ExceptionRoutine(
    PEXCEPTION_RECORD ExceptionRecord,
    PVOID EstablisherFrame,
    PCONTEXT ContextRecord,
    PVOID DispatcherContext)
{
    g_isDebuggerPresent = false;
    ContextRecord->Eip += 1;
    return ExceptionContinueExecution;
}


int main(){
    cout << "Start" << endl;
    __asm{
        push ExceptionRoutine
        push dword ptr fs:[0]
        mov dword ptr fs:[0], esp

        int 3h

        mov eax, [esp]
        mov dword ptr fs:[0], eax
        add esp, 8
    }
    if(g_isDebuggerPresent){
        cout << "Stop debugging program!" << endl;
        system("pause");
        exit(-1);
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}