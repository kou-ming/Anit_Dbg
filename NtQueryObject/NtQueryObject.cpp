#include <Windows.h>
#include <iostream>


using namespace std;

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation = 0,
    ObjectNameInformation = 1,
    ObjectTypeInformation = 2,
    ObjectAllTypesInformation = 3,
    ObjectHandleInformation = 4
} OBJECT_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _OBJECT_TYPE_INFORMATION {
    UNICODE_STRING TypeName;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_ALL_TYPES_INFORMATION {
    ULONG NumberOfObjectTypes;
    OBJECT_TYPE_INFORMATION ObjectTypeInformation[1];
} OBJECT_ALL_TYPES_INFORMATION, * POBJECT_ALL_TYPES_INFORMATION;

typedef NTSTATUS(NTAPI* _NtQueryObject)(
    HANDLE Handle,
    OBJECT_INFORMATION_CLASS ObjectInformationClass,
    PVOID ObjectInformation,
    ULONG ObjectInformationLength,
    PULONG ReturnLength
    );
_NtQueryObject NtQueryObject;


int main() {
    cout << "Start!" << endl;
    POBJECT_ALL_TYPES_INFORMATION typesInfo = NULL;
    NtQueryObject = (_NtQueryObject)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryObject");
    char* buffer = (char*)malloc(0x4000);
    DWORD realSize = 0;
    NTSTATUS ret = NtQueryObject(NULL, ObjectAllTypesInformation, buffer, 0x4000, &realSize);
    if (ret != 0) {
        cout << "NtQueryObject error!" << endl;
    }

    typesInfo = (POBJECT_ALL_TYPES_INFORMATION)buffer;
    POBJECT_TYPE_INFORMATION typeInfo = typesInfo->ObjectTypeInformation;
    for (ULONG i = 0; i < typesInfo->NumberOfObjectTypes; i++) {
        if (wcscmp(L"DebugObject", typeInfo->TypeName.Buffer) == 0) {
            if (typeInfo->TotalNumberOfObjects > 0) {
                cout << "now Debugging!!!" << endl;
                system("pause");
                ExitProcess(0);
            }
        }
        char* temp = (char*)typeInfo->TypeName.Buffer;
        temp += typeInfo->TypeName.MaximumLength;
        temp = temp + (DWORD)temp % 4;
        DWORD data = *(DWORD*)temp;
        while (data == 0) {
            temp += 4;
            data = *(DWORD*)temp;
        }
        typeInfo = (POBJECT_TYPE_INFORMATION)temp;

    }
    cout << "no Debugger" << endl;
    system("pause");
    return 0;
}