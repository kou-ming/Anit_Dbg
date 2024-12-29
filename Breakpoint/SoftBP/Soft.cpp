// cl main.cpp /INCREMENTAL:NO
#include <windows.h>
#include <iostream>

using namespace std;

DWORD CalcFuncCrc(PUCHAR funcBegin, PUCHAR funcEnd){
    DWORD crc = 0;
    for(; funcBegin < funcEnd ; ++ funcBegin){
        crc += *funcBegin;
    }
    return crc;
}

#pragma auto_inline(off)
VOID DebuggeeFunction(){
    cout << "DebuggeeFunction" << endl;
    int calc = 0;
    calc += 2;
    calc <<= 8;
    calc -= 3;
}
VOID DebuggeeFunctionEnd(){

};
#pragma auto_inline(on)
DWORD g_origCrc = 0x24A9;
int main(){
    DWORD crc = CalcFuncCrc((PUCHAR)DebuggeeFunction, (PUCHAR)DebuggeeFunctionEnd);
    cout << "now crc is: 0x" << hex << crc << endl;
    if(g_origCrc != crc){
        cout << "Stop debugging program!" << endl;
    }
    system("pause");
    return 0;
}