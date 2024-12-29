#include <windows.h>
#include <iostream>

using namespace std;

EXCEPTION_DISPOSITION ExceptionRoutine(
    PEXCEPTION_RECORD ExceptionRecord,
    PVOID EstablisherFrame,
    PCONTEXT ContextRecord,
    PVOID DispatcherContext)
{
    cout << "Enter Exception!" << endl;
    if(ExceptionRecord->ExceptionCode == EXCEPTION_INVALID_HANDLE){
        cout << "Stop debugging program!" << endl;
        system("pause");
        exit(-1);
    }
    return ExceptionContinueExecution;
}

int main() {
    cout << "Start" << endl;
    __asm{
        push ExceptionRoutine
        push dword ptr fs : [0]
        mov dword ptr fs : [0], esp
    }
    CloseHandle((HANDLE)0xBAAD);
    __asm{
        mov eax, [esp]
        mov dword ptr fs : [0], eax
        add esp, 8
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}