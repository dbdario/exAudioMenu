#pragma once
#include <windows.h>

// Source: https://stackoverflow.com/questions/1888863/how-to-get-main-window-handle-from-process-id

struct handle_data {
    DWORD process_id;
    HWND window_handle;
};

HWND find_main_window(DWORD process_id);
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam);
BOOL is_main_window(HWND handle);