#include <Windows.h>
#include <iostream>

using namespace std;

int main() {
    cout << "Start!" << endl;
    BOOL ifVM = TRUE;
    __try {
        _asm {
            push edx
            push ecx
            push ebx
            mov eax, 'VMXh'
            mov ebx, 0
            mov ecx, 10
            mov edx, 'VX'
            in eax, dx
            cmp ebx, 'VMXh'
            pop ebx
            pop ecx
            pop edx
        }
    }
    __except (1){
        ifVM = FALSE;
    }
    if (ifVM) {
        MessageBoxA(0, "檢測到虛擬機", "警告", MB_OK);
    }
    return 0;
}