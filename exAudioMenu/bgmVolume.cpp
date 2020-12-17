#include "bgmVolume.h"
// Volume information
void bgmVolController(uintptr_t bgmVolAddr, HANDLE hProcess, int volume, bool mute) {
	char newVal;
	if (mute == true) {
		newVal = 0;
		WriteProcessMemory(hProcess, (BYTE*)bgmVolAddr, &newVal, sizeof(newVal), nullptr);
		return;
	}
	if (mute == false && volume == 128) {
		mem::PatchEx((BYTE*)(bgmVolAddr), (BYTE*)"\x80\x00\x00", 3, hProcess);
		return;
	}
	else if (mute == false) {
		newVal = volume;
		WriteProcessMemory(hProcess, (BYTE*)bgmVolAddr, &newVal, sizeof(newVal), nullptr);
		return;
	}
}