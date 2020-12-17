#include "voiceVolume.h"

void voiceVolController(uintptr_t voiceVolAddr, uintptr_t ingameAddr, HANDLE hProcess, int volume, bool mute) {
	char newVal;
	int baseValue = 1006370816;
	int ingameVal;
	int maxVolume = 1065353216;
	if (mute == true) {
		newVal = 0;
		ingameVal = 0;
		WriteProcessMemory(hProcess, (BYTE*)voiceVolAddr, &newVal, sizeof(newVal), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
		return;
	}
	if (mute == false && volume == 128) {
		mem::PatchEx((BYTE*)(voiceVolAddr), (BYTE*)"\x80\x00\x00", 3, hProcess);
		WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &maxVolume, sizeof(maxVolume), nullptr);
		return;
	}
	else if (mute == false) {
		newVal = volume;
		WriteProcessMemory(hProcess, (BYTE*)voiceVolAddr, &newVal, sizeof(newVal), nullptr);
		if (volume == 0) {
			ingameVal = (int)newVal;
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume == 1) {
			ingameVal = baseValue + 262144;
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume == 2) {
			ingameVal = baseValue + 262144 + 8388608;
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume >= 3 && volume <= 4) {
			ingameVal = baseValue + 262144 + 8388608 + (4194304 * ((int)newVal - 2));
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume >= 5 && volume <= 8) {
			ingameVal = baseValue + 262144 + 8388608 + (4194304 * 2) + (2097152 * ((int)newVal - 4));
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume >= 9 && volume <= 16) {
			ingameVal = baseValue + 262144 + 8388608 + (4194304 * 2) + (2097152 * 4) + (1048576 * ((int)newVal - 8));
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume >= 17 && volume <= 32) {
			ingameVal = baseValue + 262144 + 8388608 + (4194304 * 2) + (2097152 * 4) + (1048576 * 8) + (524288 * ((int)newVal - 16));
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume >= 33 && volume <= 64) {
			ingameVal = baseValue + 262144 + 8388608 + (4194304 * 2) + (2097152 * 4) + (1048576 * 8) + (524288 * 16) + (262144 * ((int)newVal - 32));
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		if (volume >= 65 && volume <= 127) {
			ingameVal = baseValue + 262144 + 8388608 + (4194304 * 2) + (2097152 * 4) + (1048576 * 8) + (524288 * 16) + (262144 * 32) + (131072 * ((int)newVal - 64));
			WriteProcessMemory(hProcess, (BYTE*)ingameAddr, &ingameVal, sizeof(ingameVal), nullptr);
			return;
		}
		return;
	}

}