#pragma once

#include <Windows.h>
#include <string>
#include "mem.h"

void voiceVolController(uintptr_t bgmVolAddr, uintptr_t ingameAddr, HANDLE hProcess, int volume, bool mute);