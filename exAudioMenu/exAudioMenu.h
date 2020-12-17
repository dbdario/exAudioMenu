#pragma once

#include "resource.h"

// Detour
#include "detours.h"

// Header files
#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <dwmapi.h>

// Libraries
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "dwmapi.lib")

// Program Header files
#include "data.h"
#include "proc.h"
#include "mem.h"
#include "paint.h"
#include "bgmVolume.h"
#include "voiceVolume.h"
#include "seVolume.h"