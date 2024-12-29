#include <windows.h>
#include <iostream>

using namespace std;

int main(){
    cout << "Start" << endl;
    CONTEXT ctx = {};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (GetThreadContext(GetCurrentThread(), &ctx)){
        if(ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0){
            cout << "Stop debugging program!" << endl;
            system("pause");
            exit(-1);
        }
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}