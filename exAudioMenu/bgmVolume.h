#pragma once
#pragma once

#include <Windows.h>
#include <string>
#include "mem.h"
void bgmVolController(uintptr_t bgmVolAddr, HANDLE hProcess, int volume, bool mute);