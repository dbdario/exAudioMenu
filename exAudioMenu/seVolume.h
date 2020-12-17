#pragma once
#include <Windows.h>
#include "mem.h"
void seVolController(uintptr_t seVolAddr, uintptr_t ingameAddr, HANDLE hProcess, int volume, bool mute);