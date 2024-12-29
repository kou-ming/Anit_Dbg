#include <Windows.h>
#include <iostream>

using namespace std;

int main(){
    cout << "Start!!!" << endl;
    BOOL isDebuggerPresent = false;
    if(CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent)){
        if(isDebuggerPresent){
            cout << "Stop Debugging program!" << endl;
            system("pause");
            exit(-1);
        }
    }
    cout << "End~" << endl;
    system("pause");
    return 0;
}