#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);