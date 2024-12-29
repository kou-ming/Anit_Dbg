#include <windows.h>
#include <iostream>

using namespace std;

LONG CALLBACK ExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    cout << "Enter VEH" << endl;
    PCONTEXT ctx = ExceptionInfo->ContextRecord;
    if (ctx->Dr0 != 0 || ctx->Dr1 != 0 || ctx->Dr2 != 0 || ctx->Dr3 != 0) {
        cout << "Stop debugging program!" << endl;
        system("pause");
        exit(-1);
    }
    cout << ctx->Eip << endl;
    ctx->Eip += 2;
    cout << ctx->Eip << endl;
    return EXCEPTION_CONTINUE_EXECUTION;
}

int main() {
    cout << "Start VEH_" << endl;
    AddVectoredContinueHandler(0, ExceptionHandler);
     __asm int 1h;
    cout << "End~" << endl;
    system("pause");
    return 0;
}