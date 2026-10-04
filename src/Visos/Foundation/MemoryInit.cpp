#include "Control/Support/PreInit.h"
#include "Visos/Foundation/CArena.h"
#include "Visos/Foundation/CRAMArena.h"
#include "Visos/Foundation/CSmallMemory.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsInit.h"

enum {
	GLOBAL_ALLOC_FLAG_MOVEABLE = 0x0002
};

extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int p_flags, unsigned int p_bytes);
extern "C" __declspec(dllimport) void* __stdcall GlobalLock(void* p_mem);
extern "C" __declspec(dllimport) int __stdcall GlobalUnlock(void* p_mem);
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* p_mem);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLastError();

// GLOBAL: LEMBALL 0x004a2184
char g_szMasterMainRamArena[24] = "Master Main Ram Arena";

// GLOBAL: LEMBALL 0x004a219c
char g_szMasterArenaStillLocked[72] = "Master Main RAM Arena memory was locked, when it should not have been\n";

// GLOBAL: LEMBALL 0x004a21e4
char g_szMasterArenaUnlockIteratively[40] = "attempting to unlock it iteratively\n";

// GLOBAL: LEMBALL 0x004a220c
char g_szMasterArenaFreeFailed[52] = "Freeing the Master Main RAM Arena memory failed\n";

// GLOBAL: LEMBALL 0x004a2240
char g_szGetLastErrorEq[20] = "GetLastError()==";

// GLOBAL: LEMBALL 0x004a2254
char g_szColonHexPrefix[8] = ": 0x";

// GLOBAL: LEMBALL 0x004a225c
char g_szGetLastErrorNewline[4] = "\n";

// FUNCTION: LEMBALL 0x0046f060
bool _MEM_Init()
{
	void* locked;
	int smallEnabled;
	CSmallMemory* smallMemory;

	g_pMasterArenaMemory = GlobalAlloc(GLOBAL_ALLOC_FLAG_MOVEABLE, g_preInitActive.m_memoryBudget);
	if (g_pMasterArenaMemory == NULL) {
		return false;
	}

	locked = GlobalLock(g_pMasterArenaMemory);
	if (locked == NULL) {
		return false;
	}

	g_pMasterArena = new (locked) CRAMArena(g_preInitActive.m_memoryBudget, g_szMasterMainRamArena, NULL, NULL);

	smallMemory = NULL;
	smallEnabled = g_nSmallMemoryEnabled;
	g_pSmallMemory = smallMemory;
	if (smallEnabled != 0) {
		g_nSmallMemoryEnabled = 0;
		smallMemory = new CSmallMemory();
	}
	g_pSmallMemory = smallMemory;
	g_nSmallMemoryEnabled = smallEnabled;
	if (g_pMasterArena != NULL && (smallEnabled == 0 || smallMemory != NULL)) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0046f120
bool _MEM_Quit()
{
	CSmallMemory* smallMemory;
	unsigned int lastError;

	g_nSmallMemoryEnabled = 0;
	smallMemory = g_pSmallMemory;
	if (smallMemory != NULL) {
		smallMemory->~CSmallMemory();
		operator delete(smallMemory);
	}
	delete g_pMasterArena;
	if (GlobalUnlock(g_pMasterArenaMemory) != 0) {
		*g_pErrorOutput << g_szMasterArenaStillLocked;
		*g_pErrorOutput << g_szMasterArenaUnlockIteratively;
		do {
		} while (GlobalUnlock(g_pMasterArenaMemory) != 0);
	}
	if (GlobalFree(g_pMasterArenaMemory) != NULL) {
		lastError = GetLastError();
		*g_pErrorOutput << g_szMasterArenaFreeFailed;
		*g_pErrorOutput << g_szGetLastErrorEq << (unsigned long) lastError << g_szColonHexPrefix << Hex(lastError)
						<< g_szGetLastErrorNewline;
		return false;
	}
	return true;
}
