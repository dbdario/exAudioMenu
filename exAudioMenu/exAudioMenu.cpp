// exAudioMenu.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "exAudioMenu.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR overlayTitle[100] = L"Overlay";

int width   = 0;
int height  = 0;
DWORD procId = NULL;
DWORD pId = NULL;
HWND overlayHwnd = NULL;
HWND targetHwnd = NULL;
RECT rect;          // window

// Addresses
uintptr_t moduleBase = 0;		// Plus R address
uintptr_t bgmVolAddr = 0;		// BGM Volume Address
uintptr_t voiceVolAddr = 0;		// Voice Volume Address
uintptr_t seVolAddr = 0;		// Sound Effects Volume Address
uintptr_t ingameVolAddr = 0;	// In-game audio Address
uintptr_t ingameVoiceAddr = 0;	// In-game Voice Address
uintptr_t ingameSEAddr;			// In-game Sound Effects Volume Address

std::vector<unsigned int>voiceOffset = { 0x60 };	//offset for ingame voice addr
std::vector<unsigned int>seOffset = { 0x5C };		//offset for ingame SE

Paint paint;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    MyRegisterClass(hInstance);
    // === Find Guilty Gear XX Accent Core Plus R ===
    // Source: https://stackoverflow.com/questions/1888863/how-to-get-main-window-handle-from-process-id
    // 1. Get process ID (function made by GuidedHacking)
    procId = GetProcId(L"GGXXACPR_Win.exe");    //GGXXACPR_Win
    // 2. Get Plus R's handle (function provided in Source)
    targetHwnd = find_main_window(procId);
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // 3. If Plus R's handle is found, get window dimensions
    if (targetHwnd) {
        // Establish rectangle to get dimensions
        // Take dimension information from Plus R's handle and put it into rectangle
        // If "GetWindowRect()" does not work, use "DwmGetWindowAttribute()"
        // Source: https://stackoverflow.com/questions/27928254/adjustwindowrectex-and-getwindowrect-give-wrong-size-with-ws-overlapped
        GetWindowRect(targetHwnd, &rect);
        // Write dimensions
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        width -= 5;
        height -= 29;
    }
    else
        return FALSE;

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    paint = Paint(overlayHwnd, targetHwnd, hProcess, procId, width, height);

    // ======= SET UP ADDRESSES ========
    paint.moduleBase = GetModuleBaseAddress(procId, L"GGXXACPR_Win.exe");
    paint.bgmVolAddr = paint.moduleBase + 0x5D46F4;
    paint.voiceVolAddr = paint.moduleBase + 0x5D46FC;
    paint.seVolAddr = paint.moduleBase + 0x5D46F8;
    paint.ingameVolAddr = paint.moduleBase + 0xA16B2C;

    paint.ingameVoiceAddr = mem::FindDMAAddy(hProcess, paint.ingameVolAddr, paint.voiceOffset);
    paint.ingameSEAddr = mem::FindDMAAddy(hProcess, paint.ingameVolAddr, paint.seOffset);
    

    // ==== INITIALIZE VOLUME VALUES ====
    // Set initial BGM volume
    int bgmBuffer = 0;
    ReadProcessMemory(hProcess, (BYTE*)paint.bgmVolAddr, &bgmBuffer, sizeof(bgmBuffer), nullptr);
    paint.bgmSliderX = paint.rectx + 86 + bgmBuffer;
    // Set initial Voice volume
    byte voiceBuffer = 0;    // voice volume is in 2 bytes, so we can't just use int from the go
    ReadProcessMemory(hProcess, (BYTE*)paint.voiceVolAddr, &voiceBuffer, sizeof(voiceBuffer), nullptr);
    paint.voiceSliderX = paint.rectx + 86 + (int)voiceBuffer;
    // Set initial SE volume
    byte seBuffer = 0;
    ReadProcessMemory(hProcess, (BYTE*)paint.seVolAddr, &seBuffer, sizeof(seBuffer), nullptr);
    paint.seSliderX = paint.rectx + 86 + (int)seBuffer;


    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        //RECT rect;
        GetWindowRect(targetHwnd, &rect);
        paint.width = rect.right - rect.left;
        paint.height = rect.bottom - rect.top;


        MoveWindow(overlayHwnd, rect.left, rect.top, paint.width, paint.height, true);
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(RGB(0, 0, 0)); //(HBRUSH)(COLOR_WINDOW+1)
    wcex.lpszMenuName   = overlayTitle;
    wcex.lpszClassName  = overlayTitle;
    wcex.hIconSm        = 0;

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   overlayHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, overlayTitle, overlayTitle, WS_POPUP | WS_VISIBLE,
       1, 1, width, height, nullptr, nullptr, hInstance, nullptr);

   if (!overlayHwnd)
   {
      return FALSE;
   }

   // Using LWA_COLORKEY is a one-way ticket to killing frames
   // Using only LWA_ALPHA is important
   // Gonna use this method instead
   // source: https://www.unknowncheats.me/forum/anti-cheat-bypass/340062-overlay-fps-issue.html
   SetLayeredWindowAttributes(overlayHwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
   MARGINS Margin = { -1 };
   DwmExtendFrameIntoClientArea(overlayHwnd, &Margin);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        paint.render();
        
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}