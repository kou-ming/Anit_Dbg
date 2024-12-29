#include<iostream>
#include <windows.h>

using namespace std;

int main(){
    cout << "start"<<endl;
    if (IsDebuggerPresent()){
        std::cout << "Debugger is present !!!" << std::endl;
        exit(-1);
    } 
    else {
        std::cout << "Debugger is not present :)" << std::endl;
    }
    return 0;
}