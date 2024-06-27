#ifndef HEADER_H
#define HEADER_H


#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <conio.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <comdef.h>
#include <vector>
#include <tchar.h>
#include <stdlib.h>
#include <stdint.h>
#include <uxtheme.h>

#pragma comment(lib, "uxtheme.lib")
#define UNICODE 
#define WIN32_LEAN_AND_MEAN 

#define IDC_BUTTON 1001
#define IDC_BUTTON_FREEZE 1002 

#define IDC_BUTTON_CASH 1003 
#define IDC_BUTTON_FREEZE_CASH 1004

#define IDC_BUTTON_MAN 1005 
#define IDC_BUTTON_FREEZE_MAN 1006

#define IDC_YOURLABELID 1007 

#define IDC_BUTTON_SVVA 1008
#define IDC_BUTTON_SVVD 1009
#define IDC_BUTTON_SVVM 1010
#define IDC_BUTTON_SVNA 1011
#define IDC_BUTTON_SVND 1012
#define IDC_BUTTON_SVNM 1013

using namespace std;

namespace VARS {
	DWORD GetProcess(const wchar_t* Target) {
		HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshotHandle == INVALID_HANDLE_VALUE) {
			return NULL;
		}
		PROCESSENTRY32W processEntry = { };
		processEntry.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(snapshotHandle, &processEntry)) {
			do {
				if (_wcsicmp(processEntry.szExeFile, Target) == 0) {
					CloseHandle(snapshotHandle);
					return processEntry.th32ProcessID;
				}
			} while (Process32NextW(snapshotHandle, &processEntry));
		}
		CloseHandle(snapshotHandle);
		return NULL;
	}


	uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* ModuleTarget) {
		HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
		if (snapshotHandle == INVALID_HANDLE_VALUE) {
			return NULL;
		}
		MODULEENTRY32W moduleEntry = { };
		moduleEntry.dwSize = sizeof(MODULEENTRY32W);
		if (Module32FirstW(snapshotHandle, &moduleEntry)) {
			do {
				if (_wcsicmp(moduleEntry.szModule, ModuleTarget) == 0) {
					CloseHandle(snapshotHandle);
					return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
				}
			} while (Module32NextW(snapshotHandle, &moduleEntry));
		}
		CloseHandle(snapshotHandle);
		return NULL;
	}


	DWORD processId = GetProcess(L"eu4.exe");
	uintptr_t baseAddress = GetModuleBaseAddress(processId, L"eu4.exe");
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);


	template <typename type>
	type memRead(uintptr_t pointerStatic) {
		type value = { };
		ReadProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);
		return value;
	}

	template <typename type>
	bool memWrite(uintptr_t pointerStatic, type value) {
		return WriteProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);  
	}
}

namespace offsets
{
	DWORD offModuleBase = 0x0249EA00;
	DWORD offBase = 0x118;
	DWORD offModuleProvince = 0x024A1048;
	DWORD offBaseProvince = 0x6F0;

	DWORD offModuleForIterator = 0x24A0FE8;
	DWORD offForIterator = 0x1E60;

	DWORD offAdm = 0x260;
	DWORD offDip = 0x264;
	DWORD offMil = 0x268;

	DWORD offCash = 0xBB0;
	DWORD offManPower = 0xDB8;
	DWORD offSeaPower = 0xDC0;
	DWORD offStability = 0xBD8;

	DWORD offCountryName = 0x20;
	DWORD offCountryId = 0x24;

	DWORD offProvinceName = 0x390;
	DWORD offProvinceId = 0x20;
	DWORD offProvinceOwner = 0x46C; 
	DWORD offProvincecapital = 0x2DDC;

	DWORD offProvinceAdm = 0x3E4;
	DWORD offProvinceDip = 0x3E8;
	DWORD offProvinceMil = 0x3E0;
}

#endif