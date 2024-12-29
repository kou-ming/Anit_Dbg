// cl /EHsc /MD .\TLS_callback.cpp user32.lib
#include "windows.h"
#include <stdio.h>

void NTAPI __stdcall TLSCallbacks(PVOID DllHandle, DWORD dwReason, PVOID Reserved);

#ifdef _M_IX86
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:__tls_callback")
#else
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback")
#endif
EXTERN_C
#ifdef _M_X64
#pragma const_seg (".CRT$XLB")
const
#else
#pragma data_seg (".CRT$XLB")
#endif

PIMAGE_TLS_CALLBACK _tls_callback = TLSCallbacks;
#pragma data_seg ()
#pragma const_seg ()

void NTAPI __stdcall TLSCallbacks(PVOID DllHandle, DWORD dwReason, PVOID Reserved)
{
	if (IsDebuggerPresent()) {
		MessageBoxA(nullptr, "TLS Callback", "", 0);
		ExitProcess(0);
	}
}

int main(int argc, char* argv[])
{
	printf("Main function!");
}
