#include <iostream>
#include <windows.h>
#include <winnt.h>

using namespace std;

PIMAGE_NT_HEADERS GetImageNtHeaders(PBYTE pImageBase){
    PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)pImageBase;
    return (PIMAGE_NT_HEADERS)(pImageDosHeader + pImageDosHeader->e_lfanew);
}

PIMAGE_SECTION_HEADER FindRDataSection(PBYTE pImageBase){
    static const string rdata = ".rdata";
    PIMAGE_NT_HEADERS pImageNtHeader = GetImageNtHeaders(pImageBase);
    PIMAGE_SECTION_HEADER pImageSectionHeader = IMAGE_FIRST_SECTION(pImageNtHeader);
    int n = 0;
    for( ; n < pImageNtHeader->FileHeader.NumberOfSections ; n++){
        if(rdata == (char *)pImageSectionHeader[n].Name){
            break;
        }
    }
    return &pImageSectionHeader[n];
}

void CheckGlobalFlagsClearInProcess(){
    cout << "[+] Start Check" <<endl;
    PBYTE pImageBase = (PBYTE)GetModuleHandle(NULL);
    if(!pImageBase){
        exit(-1);
    }
    cout << "[+] Get PImageBase: " << static_cast<void*>(pImageBase) << endl;
    PIMAGE_NT_HEADERS pImageNtHeaders = GetImageNtHeaders(pImageBase);
    if(!pImageNtHeaders){
        exit(-1);
    }
    cout << "[+] Get NtHeader" << endl;
    PIMAGE_LOAD_CONFIG_DIRECTORY pImageLoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)(pImageBase + pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
    // PIMAGE_LOAD_CONFIG_DIRECTORY pImageLoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)(pImageBase + 0x3A450);
    if(!pImageLoadConfigDirectory){
        exit(-1);
    }
    cout << "[+] Get PIMAGE_LOAD_CONFIG_DIRECTORY: " << pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress << endl;
    cout <<  pImageLoadConfigDirectory << endl;
    system("pause");
    if(pImageLoadConfigDirectory->GlobalFlagsClear != 0){
        cout << "Stop debugging program!" << endl;
        system("pause");
        exit(-1);
    }
    cout << "[+] End CheckGlobalFlagClrearInProcess" << endl;
}

void CheckGlobalFlagsClearInFile()
{
    HANDLE hExecutable = INVALID_HANDLE_VALUE;
    HANDLE hExecutableMapping = NULL;
    PBYTE pMappedImageBase = NULL;
    __try
    {
        PBYTE pImageBase = (PBYTE)GetModuleHandle(NULL);
        PIMAGE_SECTION_HEADER pImageSectionHeader = FindRDataSection(pImageBase);
        TCHAR pszExecutablePath[MAX_PATH];
        DWORD dwPathLength = GetModuleFileName(NULL, pszExecutablePath, MAX_PATH);
        if (0 == dwPathLength) __leave;
        hExecutable = CreateFile(pszExecutablePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (INVALID_HANDLE_VALUE == hExecutable) __leave;
        hExecutableMapping = CreateFileMapping(hExecutable, NULL, PAGE_READONLY, 0, 0, NULL);
        if (NULL == hExecutableMapping) __leave;
        pMappedImageBase = (PBYTE)MapViewOfFile(hExecutableMapping, FILE_MAP_READ, 0, 0,
            pImageSectionHeader->PointerToRawData + pImageSectionHeader->SizeOfRawData);
        if (NULL == pMappedImageBase) __leave;
        PIMAGE_NT_HEADERS pImageNtHeaders = GetImageNtHeaders(pMappedImageBase);
        PIMAGE_LOAD_CONFIG_DIRECTORY pImageLoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)(pMappedImageBase 
            + (pImageSectionHeader->PointerToRawData
                + (pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress - pImageSectionHeader->VirtualAddress)));
        if (pImageLoadConfigDirectory->GlobalFlagsClear != 0)
        {
            std::cout << "Stop debugging program!" << std::endl;
            exit(-1);
        }
    }
    __finally
    {
        if (NULL != pMappedImageBase)
            UnmapViewOfFile(pMappedImageBase);
        if (NULL != hExecutableMapping)
            CloseHandle(hExecutableMapping);
        if (INVALID_HANDLE_VALUE != hExecutable)
            CloseHandle(hExecutable);
    } 
}

int main(){
    cout << "start" << endl;
    CheckGlobalFlagsClearInProcess();
    cout << "No Debugging Now" <<endl;
    system("pause");
    return 0;
}
