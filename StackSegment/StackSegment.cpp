#include <windows.h>
#include <iostream>

using namespace std;

int main() {
    cout << "start" << endl;
    __asm{
        push ss
        pop ss
        mov eax, 0x001
        xor edx, edx
    }
    cout << "end~" << endl;
    system("pause");
    return 0;
}