#pragma once

#include <string> //save error
#include <Windows.h>
#include <vector>
#include "proc.h"
#include "mem.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <DxErr.h> //get error from error code
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include "bgmVolume.h"
#include "voiceVolume.h"
#include "seVolume.h"

class Paint {
public:
	// On or off
	bool power = false;
	// Reset volume values
	bool resetFlag = false;
	
	IDirect3D9Ex* d3dObject = NULL;			// Used to create device
	IDirect3DDevice9Ex* d3dDevice = NULL;	// Contains functions like begin and end scene
	D3DPRESENT_PARAMETERS d3dparams;		// parameters for creating device
	ID3DXFont* d3dFontTitle = 0;			// font used when displaying text
	ID3DXFont* d3dFontSub = 0;				// font used when displaying text
	ID3DXFont* d3dFontText = 0;
	HWND targetWnd;
	DWORD procId;
	HANDLE hProcess;
	ID3DXLine* line = nullptr;
	int width;
	int height;

	// Menu
	bool iter = false;
	int rectx = 100, recty = 50, rectw = 400, recth = 430;  // menu dimensions
	int padding = 20;

	// Cursor
	POINT Pos;
	int cursorx, cursory;
	bool clicking = false;
	bool prevClicking = false;
	bool holdClicking = false;
	bool prevHoldClicking = false;

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

	// ===== MUTE VALUES =====
	bool bgmMute = false;
	bool voiceMute = false;
	bool seMute = false;

	//  === Slider Values ===
	// BGM Slider
	float bgmSliderX = 0;
	float bgmSliderValue = 0;
	char bgmSliderValue_char[101];
	// Voice Slider
	float voiceSliderX = 0;
	float voiceSliderValue = 0;
	char voiceSliderValue_char[101];
	// Sound Effects Slider
	float seSliderX = 0;
	float seSliderValue = 0;
	char seSliderValue_char[101];

	int d3D9Init(HWND hWnd);
	void MouseInput();
	void drawTitleText(char* String, int x, int y, D3DCOLOR color, int fontSize);
	void drawSubText(char* String, int x, int y, D3DCOLOR color, int fontSize);
	void drawText(const char* text, float x, float y, D3DCOLOR color);
	void drawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void drawRect(int x, int y, int width, int height, D3DCOLOR color);
	void drawButton(int x, int y, int w, int h, const char* message);

	void DrawResetButton(int x, int y, int w, int h, const char* message);

	void DrawBgmMuteButton(int x, int y, int w, int h, const char* message);
	void DrawVoiceMuteButton(int x, int y, int w, int h, const char* message);
	void DrawSEMuteButton(int x, int y, int w, int h, const char* message);
	void drawBgmSlider(float x, float y, float w, int min, int max);
	void drawVoiceSlider(float x, float y, float w, int min, int max);
	void drawSeSlider(float x, float y, float w, int min, int max);
	
	
	void displayVolVal();
	void drawStaticMenu();

	Paint();
	Paint(HWND hWnd, HWND targetWnd, HANDLE hProc, DWORD pId, int width, int height);
	int render();

};

