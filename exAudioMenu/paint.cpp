#include "paint.h"

int Paint::d3D9Init(HWND hWnd) {
	
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject))) {
		exit(1);
	}

	ZeroMemory(&d3dparams, sizeof(d3dparams));

	d3dparams.BackBufferWidth = width;
	d3dparams.BackBufferHeight = height;
	d3dparams.hDeviceWindow = hWnd;
	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dparams.EnableAutoDepthStencil = TRUE;
	d3dparams.AutoDepthStencilFormat = D3DFMT_D16;
	d3dparams.Windowed = FALSE;


	HRESULT result = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);
	
	// Make game work if game is windowed
	if (FAILED(result)) {
		d3dparams.Windowed = !d3dparams.Windowed;
		HRESULT result = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);

		if (FAILED(result)) {
			std::wstring ws(DXGetErrorString(result));
			std::string str(ws.begin(), ws.end());
			std::wstring ws2(DXGetErrorDescription(result));
			std::string str2(ws2.begin(), ws2.end());
			std::string error = "Error: " + str + " error description: " + str2;
			exit(1);
		}
	}

	return 0;
}

Paint::Paint() {};

Paint::Paint(HWND hWnd, HWND targetWnd, HANDLE hProc, DWORD pId, int width, int height) {
	this->width = width;
	this->height = height;
	this->targetWnd = targetWnd;
	this->hProcess = hProc;
	this->procId = pId;
	d3D9Init(hWnd);
}

// Source: https://www.unknowncheats.me/forum/direct3d/112935-snippets-your-menu-checkboxes-sliders-buttons-mouse.html
void Paint::MouseInput() {
	RECT rMyRect;

	GetWindowRect(targetWnd, (LPRECT)&rMyRect);
	GetCursorPos(&Pos);
	cursorx = Pos.x - 00 - rMyRect.left;
	cursory = Pos.y - 0 - rMyRect.top;
}

int Paint::render()
{
	Sleep(1000 / 60);
	if (d3dDevice == nullptr)
		return 1;
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	d3dDevice->BeginScene();
	
	if (targetWnd == GetForegroundWindow())
	{
		MouseInput();
		
		// ====== ACTIVATE MENU ======
		// If 1 is pressed, pull up menu
		// Otherwise, remove it
		if (GetAsyncKeyState('1') & 0x01) {
			if (!power) {
				power = true;
			}
			else {
				d3dDevice->Release();
				power = false;
			}
		}
		// ======= MOUSE CLICK =======
		// Clicking status
		if ((iter == true) && ((prevClicking) && !holdClicking)) {
			prevClicking = false;
		}
		// click left mouse button
		if (GetAsyncKeyState(VK_LBUTTON) & 1) {
			clicking = true;
		}
		// holding left mouse button
		else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			clicking = false;
			holdClicking = true;
			if (iter ==  true) {
				prevClicking = true;
			}
		}
		else {
			clicking = false;
			holdClicking = false;
		}
		// ====== DRAW MENU ======
		// If 1 is pressed, pull up menu
		// Otherwise, remove it
		if (power) {
			// Draw Manu Background
			drawStaticMenu();
			// Draw BGM Controller
			DrawBgmMuteButton(rectx + 41, recty + 110, 20, 20, "Mute");
			drawBgmSlider(rectx + 91, recty + 113, 128, 0, 128);	// min = 0, max = 128
			// Draw Voice Controller
			DrawVoiceMuteButton(rectx + 41, recty + 210, 20, 20, "Mute");
			drawVoiceSlider(rectx + 91, recty + 213, 128, 0, 128);
			// Draw Sound Effects Controller
			DrawSEMuteButton(rectx + 41, recty + 310, 20, 20, "Mute");
			drawSeSlider(rectx + 91, recty + 313, 128, 0, 128);
			// Draw Reset button
			DrawResetButton(rectx+41, recty + 360, 60, 30, "Default");
			displayVolVal();
		}
		if (iter == false) {
			iter = true;
		}
		
	}
	

	d3dDevice->EndScene();
	d3dDevice->Present(NULL, NULL, NULL, NULL);

	return 0;
}

void Paint::drawTitleText(char* String, int x, int y, D3DCOLOR color, int fontSize)
{
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	if (!d3dFontTitle)
		D3DXCreateFont(d3dDevice, fontSize, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &d3dFontTitle);
	d3dFontTitle->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, color);
}

void Paint::drawSubText(char* String, int x, int y, D3DCOLOR color, int fontSize) {
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	if (!d3dFontSub)
		D3DXCreateFont(d3dDevice, fontSize, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &d3dFontSub);
	d3dFontSub->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, color);

}

void Paint::drawText(const char* text, float x, float y, D3DCOLOR color) {
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	if (!d3dFontText)
		D3DXCreateFont(d3dDevice, 20, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &d3dFontText);
	d3dFontText->DrawTextA(0, text, strlen(text), &FontPos, DT_NOCLIP, color);
}

void Paint::drawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DXVECTOR2 vertices[2] = { D3DXVECTOR2(x + w / 2, y),D3DXVECTOR2(x + w / 2, y + h) };
	if (!line) {
		D3DXCreateLine(d3dDevice, &line);
	}
	line->SetWidth(w);
	line->Draw(vertices, 2, color);
}

void Paint::drawRect(int x, int y, int width, int height, D3DCOLOR color) {
	D3DXVECTOR2 Rect[5];
	Rect[0] = D3DXVECTOR2(x, y);
	Rect[1] = D3DXVECTOR2(x + width, y);
	Rect[2] = D3DXVECTOR2(x + width, y + height);
	Rect[3] = D3DXVECTOR2(x, y + height);
	Rect[4] = D3DXVECTOR2(x, y);
	if (!line) {
		D3DXCreateLine(d3dDevice, &line);
	}
	line->SetWidth(1);
	line->Draw(Rect, 5, color);
}

void Paint::drawButton(int x, int y, int w, int h, const char* message) {
	
	POINT Pos;
	GetCursorPos(&Pos);
	RECT rMyRect;
	GetWindowRect(targetWnd, &rMyRect);
	cursorx = Pos.x - rMyRect.left;
	cursory = Pos.y - rMyRect.top;

	drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(255, 153, 255, 153));
	drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 0, 0, 0));
	drawText(message, x, y + 30, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (cursorx > x&& cursory > y + (h / 3) && cursorx <= x + w && cursory <= y + (h + (h/2) + (h/4))) {
		drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}


void Paint::displayVolVal() {
	// Display BGM Volume
	bgmSliderValue = bgmSliderX - rectx - 86;	// BGM volume's actual value
	sprintf_s(bgmSliderValue_char, "  %1.0f", bgmSliderValue);
	drawText(bgmSliderValue_char, rectx + 91 + 128 + 20, recty + 113 - 0, D3DCOLOR_ARGB(255, 255, 80, 80));

	// Display Voice Volume
	voiceSliderValue = voiceSliderX - rectx - 86;
	sprintf_s(voiceSliderValue_char, "  %1.0f", voiceSliderValue);
	drawText(voiceSliderValue_char, rectx + 91 + 128 + 20, recty + 213 - 0, D3DCOLOR_ARGB(255, 255, 80, 80));

	// Display Sound Effects Volume
	seSliderValue = seSliderX - rectx - 86;
	sprintf_s(seSliderValue_char, "  %1.0f", seSliderValue);
	drawText(seSliderValue_char, rectx + 91 + 128 + 20, recty + 313 - 0, D3DCOLOR_ARGB(255, 255, 80, 80));
}

void Paint::DrawResetButton(int x, int y, int w, int h, const char* message) {
	GetCursorPos(&Pos);
	RECT rMyRect;
	GetWindowRect(targetWnd, (LPRECT)&rMyRect);

	// ===== DRAW DEFAULT BUTTON =====
	// Reset button
	drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(255, 153, 255, 153));
	// Outline of Mute box
	drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 0, 0, 0));
	// Mute box label
	drawText(message, x + 3, y + 5, D3DCOLOR_ARGB(255, 125, 125, 125));
	
	// === INTERACTION WITH MUTE BOX ===
	if (cursorx > x&& cursory > y&& cursorx <= x + w && cursory <= y + h + (h / 2))
	{
		drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		if (holdClicking) {
			drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (((prevClicking) && !holdClicking))
		{
			//resetFlag = true;
			// BGM reset to 48
			bgmSliderX = rectx + 86 + 48;
			//bgmVolController(bgmVolAddr, hProcess, 48, false);
			// Voice reset to 47
			voiceSliderX = rectx + 86 + 47;
			//voiceVolController(voiceVolAddr, ingameVolAddr, hProcess, 47, false);
			// Sound effects reset to 40
			seSliderX = rectx + 86 + 40;
			//seVolController(seVolAddr, ingameSEAddr, hProcess, 40, false);
			return;
		}
	}
}

void Paint::DrawBgmMuteButton(int x, int y, int w, int h, const char* message) {

	GetCursorPos(&Pos);
	RECT rMyRect;
	GetWindowRect(targetWnd, (LPRECT)&rMyRect);

	// ===== DRAW BGM MUTE BOX =====
	// Mute box
	drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(128, 0, 0, 0));
	// Filled in mute box square
	if (bgmMute == true) {
		drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	// Outline of Mute box
	drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 0, 0, 0));
	// Mute box label
	drawText(message, x, y + 30, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	// === INTERACTION WITH MUTE BOX ===
	if (cursorx > x&& cursory > y && cursorx <= x + w && cursory <= y + h + (h / 2))
	{
		drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (((prevClicking) && !holdClicking) && !bgmMute)
		{
			bgmMute = true;
			return;
		}
		if (((prevClicking) && !holdClicking) && bgmMute) {
			bgmMute = false;
			return;
		}
	}
}

void Paint::DrawVoiceMuteButton(int x, int y, int w, int h, const char* message) {

	GetCursorPos(&Pos);
	RECT rMyRect;
	GetWindowRect(targetWnd, (LPRECT)&rMyRect);

	// ===== DRAW VOICE MUTE BOX =====
	// Mute box
	drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(128, 0, 0, 0));
	// Filled in mute box square
	if (voiceMute == true) {
		drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	// Outline of Mute box
	drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 0, 0, 0));
	// Mute box label
	drawText(message, x, y + 30, D3DCOLOR_ARGB(255, 255, 255, 255));

	// === INTERACTION WITH MUTE BOX ===
	if (cursorx > x&& cursory > y&& cursorx <= x + w && cursory <= y + h + (h/2))
	{
		drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (((prevClicking) && !holdClicking) && !voiceMute)
		{
			voiceMute = true;
			return;
		}
		if (((prevClicking) && !holdClicking) && voiceMute) {
			voiceMute = false;
			return;
		}
	}
}

void Paint::DrawSEMuteButton(int x, int y, int w, int h, const char* message) {

	GetCursorPos(&Pos);
	RECT rMyRect;
	GetWindowRect(targetWnd, (LPRECT)&rMyRect);

	// ===== DRAW SOUND EFFECTS MUTE BOX =====
	// Mute box
	drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(128, 0, 0, 0));
	// Filled in mute box square
	if (seMute == true) {
		drawFilledRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	// Outline of Mute box
	drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 0, 0, 0));
	// Mute box label
	drawText(message, x, y + 30, D3DCOLOR_ARGB(255, 255, 255, 255));

	// === INTERACTION WITH MUTE BOX ===
	if (cursorx > x&& cursory > y&& cursorx <= x + w && cursory <= y + h + (h / 2))
	{
		drawRect(x, y, w, h, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (((prevClicking) && !holdClicking) && !seMute)
		{
			seMute = true;
			return;
		}
		if (((prevClicking) && !holdClicking) && seMute) {
			seMute = false;
			return;
		}
	}
}

void Paint::drawBgmSlider(float x, float y, float w, int min, int max) {
	float sliderw = 10.f;
	float sliderh = 15.f;

	float clickx = bgmSliderX - cursorx;
	float moveto = bgmSliderX - clickx - sliderw / 2;
	if ((cursorx >= x - (sliderw / 2)) && (cursory >= y + (sliderh/2)) && (cursorx <= x + w + (sliderw / 2)) && (cursory <= y + sliderh + (sliderh/2))) {
		
		if (holdClicking) {
			bgmSliderX = moveto;
		}
	}

	if (bgmSliderX < x - (sliderw / 2)) {
		bgmSliderX = x - (sliderw / 2);
	}
	if (bgmSliderX > x + w - (sliderw / 2)) {
		bgmSliderX = x + w - (sliderw / 2);
	}

	//Time To Draw
	drawButton(bgmSliderX, y, sliderw, sliderh, " ");
	bgmSliderValue = bgmSliderX - rectx - 86;
	bgmVolController(bgmVolAddr, hProcess, bgmSliderValue, bgmMute);
}

void Paint::drawVoiceSlider(float x, float y, float w, int min, int max) {
	float sliderw = 10.f;
	float sliderh = 15.f;

	float clickx = voiceSliderX - cursorx;
	float moveto = voiceSliderX - clickx - sliderw / 2;
	if ((cursorx >= x - (sliderw / 2)) && (cursory >= y + 2 + (sliderh / 2)) && (cursorx <= x + w + (sliderw / 2)) && (cursory <= y + 2 + sliderh + (sliderh / 2))) {
		if (holdClicking) {
			voiceSliderX = moveto;
		}
	}

	if (voiceSliderX < x - (sliderw / 2)) {
		voiceSliderX = x - (sliderw / 2);
	}
	if (voiceSliderX > x + w - (sliderw / 2)) {
		voiceSliderX = x + w - (sliderw / 2);
	}
	//Time To Draw
	drawButton(voiceSliderX, y, sliderw, sliderh, " ");
	voiceSliderValue = voiceSliderX - rectx - 86;
	voiceVolController(voiceVolAddr, ingameVoiceAddr, hProcess, voiceSliderValue, voiceMute);
}

void Paint::drawSeSlider(float x, float y, float w, int min, int max) {
	float sliderw = 10;
	float sliderh = 15;

	float clickx = voiceSliderX - cursorx;
	float moveto = voiceSliderX - clickx - sliderw / 2;
	if ((cursorx >= x - (sliderw / 2)) && (cursory >= y + 4 + (sliderh / 2)) && (cursorx <= x + w + (sliderw / 2)) && (cursory <= y + 4 + sliderh + (sliderh / 2))) {
		if (holdClicking) {
			seSliderX = moveto;
		}
	}

	if (seSliderX < x - (sliderw / 2)) {
		seSliderX = x - (sliderw / 2);
	}
	if (seSliderX > x + w - (sliderw / 2)) {
		seSliderX = x + w - (sliderw / 2);
	}

	//Time To Draw
	drawButton(seSliderX, y, sliderw, sliderh, " ");
	seSliderValue = seSliderX - rectx - 86;
	seVolController(seVolAddr, ingameSEAddr, hProcess, seSliderValue, seMute);
}

void Paint::drawStaticMenu() {
	// ======== Menu ========
	// X starting point: 100		Y starting point: 50
	// Menu width: 400				Menu height: 430
	// Padding Value: 20
	
	// Menu Backdrop
	drawFilledRect(rectx, recty, rectw, recth, D3DCOLOR_ARGB(180, 100, 149, 237));
	drawRect(rectx, recty, rectw, recth, D3DCOLOR_ARGB(230, 230, 230, 230));
	// Menu Label
	drawTitleText((char*)"AUDIO VOLUME", rectx + padding + 5, recty + padding + 5, D3DCOLOR_ARGB(255, 153, 255, 153), 45);
	// ======== BGM ========
	// BGM Label 
	drawSubText((char*)"BGM VOLUME", rectx + padding + 20, recty + padding + 50, D3DCOLOR_ARGB(255, 255, 255, 255), 30);
	// BGM Slider Spot
	drawFilledRect(rectx + 91, recty + 113, 128, 15, D3DCOLOR_ARGB(128, 0, 0, 0));
	drawRect(rectx + 91, recty + 113, 128, 15, D3DCOLOR_ARGB(255, 0, 0, 0));
	// BGM Volume Display
	drawFilledRect(rectx + 91 + 128 + 25, recty + 113 - 2, 37, 20, D3DCOLOR_ARGB(240, 70, 70, 70));
	drawRect(rectx + 91 + 128 + 25, recty + 113 - 2, 37, 20, D3DCOLOR_ARGB(255, 230, 230, 230));

	// ======= VOICE ========
	// Voice Label
	drawSubText((char*)"VOICE VOLUME", rectx + padding + 20, recty + padding + 150, D3DCOLOR_ARGB(255, 255, 255, 255), 30);
	// Voice Slider Spot
	drawFilledRect(rectx + 91, recty + 213, 128, 15, D3DCOLOR_ARGB(128, 0, 0, 0));
	drawRect(rectx + 91, recty + 213, 128, 15, D3DCOLOR_ARGB(255, 0, 0, 0));
	// Voice Volume Display
	drawFilledRect(rectx + 91 + 128 + 25, recty + 213 - 2, 37, 20, D3DCOLOR_ARGB(240, 70, 70, 70));
	drawRect(rectx + 91 + 128 + 25, recty + 213 - 2, 37, 20, D3DCOLOR_ARGB(255, 230, 230, 230));

	// === SOUND EFFECTS ====
	// SE Label
	drawSubText((char*)"SOUND EFFECTS VOLUME", rectx + padding + 20, recty + padding + 250, D3DCOLOR_ARGB(255, 255, 255, 255), 30);
	// SE Slider Spot
	drawFilledRect(rectx + 91, recty + 313, 128, 15, D3DCOLOR_ARGB(128, 0, 0, 0));
	drawRect(rectx + 91, recty + 313, 128, 15, D3DCOLOR_ARGB(255, 0, 0, 0));

	// SE Volume Display
	drawFilledRect(rectx + 91 + 128 + 25, recty + 313 - 2, 37, 20, D3DCOLOR_ARGB(240, 70, 70, 70));
	drawRect(rectx + 91 + 128 + 25, recty + 313 - 2, 37, 20, D3DCOLOR_ARGB(255, 230, 230, 230));
}