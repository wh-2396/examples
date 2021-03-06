// 枚举PEB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PEB.h"
#include <iostream>

using namespace std;





int main()
{
    HANDLE m_ProcessHandle;
    int PID;
    cout << "输入PID：";
    cin >> PID;

    m_ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    BOOL bSource = FALSE;
    BOOL bTarget = FALSE;
    //判断自己的位数
    IsWow64Process(GetCurrentProcess(), &bSource);
    //判断目标的位数
    IsWow64Process(m_ProcessHandle, &bTarget);

    if(bTarget == FALSE && bSource == TRUE)
    {
        HMODULE NtdllModule = GetModuleHandle(L"ntdll.dll");
        pfnNtWow64QueryInformationProcess64 NtWow64QueryInformationProcess64 = (pfnNtWow64QueryInformationProcess64)GetProcAddress(NtdllModule, "NtWow64QueryInformationProcess64");
        pfnNtWow64ReadVirtualMemory64 NtWow64ReadVirtualMemory64 = (pfnNtWow64ReadVirtualMemory64)GetProcAddress(NtdllModule, "NtWow64ReadVirtualMemory64");

        PROCESS_BASIC_INFORMATION64 pbi = { 0 };
        UINT64 ReturnLength = 0;

        NTSTATUS Status = NtWow64QueryInformationProcess64(m_ProcessHandle, ProcessBasicInformation, &pbi, (UINT32)sizeof(pbi), (UINT32*)&ReturnLength);

        if (NT_SUCCESS(Status))
        {

            _PEB64* Peb = (_PEB64*)malloc(sizeof(_PEB64));
            RTL_USER_PROCESS_PARAMETERS64* ProcessParameters = (RTL_USER_PROCESS_PARAMETERS64*)malloc(sizeof(RTL_USER_PROCESS_PARAMETERS64));
            Status = NtWow64ReadVirtualMemory64(m_ProcessHandle, (PVOID64)pbi.PebBaseAddress,
                (_PEB64*)Peb, sizeof(_PEB64), &ReturnLength);


            cout << "PebBaseAddress:" << hex << pbi.PebBaseAddress << endl;
            cout << "Ldr:" << hex << Peb->Ldr << endl;
            cout << "ImageBaseAddress:" << hex << Peb->ImageBaseAddress << endl;
        }
    }

    //自己是32  目标是32
    else if (bTarget == TRUE && bSource == TRUE)
    {
        HMODULE NtdllModule = GetModuleHandle(L"ntdll.dll");
        pfnNtQueryInformationProcess NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(NtdllModule,
            "NtQueryInformationProcess");
        PROCESS_BASIC_INFORMATION32 pbi = { 0 };
        UINT32  ReturnLength = 0;
        NTSTATUS Status = NtQueryInformationProcess(m_ProcessHandle,
            ProcessBasicInformation, &pbi, (UINT32)sizeof(pbi), (UINT32*)&ReturnLength);
        if (NT_SUCCESS(Status))
        {
            _PEB32* Peb = (_PEB32*)malloc(sizeof(_PEB32));
            ReadProcessMemory(m_ProcessHandle, (PVOID)pbi.PebBaseAddress, (_PEB32*)Peb, sizeof(_PEB32), NULL);
            printf("PEB:%x\r\n", pbi.PebBaseAddress);
            printf("LdrAddress:%x\r\n", ((_PEB32*)Peb)->Ldr);
            printf("ImageBaseAddress:%x\r\n", ((_PEB32*)Peb)->ImageBaseAddress);
        }
    }

    
        

    return 0;
}
