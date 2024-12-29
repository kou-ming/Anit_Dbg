#include <Windows.h>
#include <iostream>

LONG WINAPI myhandledExceptionFilter(
  _EXCEPTION_POINTERS *ExceptionInfo
)
{
    cout << "發現異常" << endl;
    ExceptionInfo -> ContextRecord -> Eip += 3;
    return EXCEPTION_CONTINUE_EXECUTION;
}

int main(){
    SetUnhandledExceptionFilter(myhandledExceptionFilter);
    __asm{
        mov eax, 0;
        mov [eax], 1;
    }
    system("pause");
    return 0;
}