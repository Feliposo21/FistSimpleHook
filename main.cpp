#include <Windows.h>
#include <basetsd.h>
#include "Ricardo.h"

extern "C" {void  OurFunct(); }
typedef unsigned __int64 QWORD;

bool Hook(void * toHook, void * OurFunct, int len)
{
	if (len < 5) {
		return false;	
	}

	DWORD curProtection;

	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD64 relativeadress = ((DWORD64)OurFunct - (DWORD64)toHook) - 5;


	*(BYTE*)toHook = 0xE9;
	*(DWORD64*)((DWORD64)toHook + 1) = relativeadress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}

//extern DWORD64 jmpBackAddy;
//void  ourFunct() {
	//mam();
//}

DWORD WINAPI MainThread(LPVOID param) {
	int hookLenght = 6;
	DWORD hookAddress = 0x7FF721F124AA;
	jmpBackAddy = hookAddress + hookLenght;

	Hook((void*)hookAddress, OurFunct, hookLenght); //ourFunct before

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) break;
		Sleep(50);
	}

	FreeLibraryAndExitThread((HMODULE)param, 0);

	return 0;
}

DWORD WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	}
	
	return TRUE;

}