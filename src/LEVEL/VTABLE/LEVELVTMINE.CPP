#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/WIN32.H"
#include "LEVEL/CHUNKOBJVT.H"

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct MineChunkObjectView : LevelChunkObjectBaseView {
	MineChunkObjectView* ConstructMineChunkObject(void);
};

struct MineChunkObjectActionView {
	virtual void Reserved0(void);
	virtual void Reserved1(void);
	virtual void SetManagedEntityStateId(int nStateId);
};

struct MineChunkObjectNotifyView {};
typedef void (MineChunkObjectNotifyView::*MineNotifyProc)(int nValue);
struct MineChunkObjectNotifyVtable {
	void* m_apReserved00[13];
	MineNotifyProc m_pNotify34;
};

struct LevelTileGridOwnerView {
	void SetCellType(int x, int y, int nType, int nVariant);
};

extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTimeMs;
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBase24CF0(void* pObject);
void* LEMBALL_FASTCALL DeleteMineChunkObjectArrayOrScalar(void* pObject, void* pUnused, unsigned int fDelete);
int LEMBALL_FASTCALL AdvanceMineChunkObjectStateMachine(void* pObject);

// GLOBAL: LEMBALL 0x00495be8
void* g_LEVELVT_MineChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteMineChunkObjectArrayOrScalar, AdvanceMineChunkObjectStateMachine);

extern int g_nLevelFrameClockTick;
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct MineChunkObjectRuntimeView {
	unsigned char m_abReserved00[0xb8];
	int m_nStateB8;
	unsigned char m_abReservedBC[0x10];
	int m_nFrameTickCC;
	unsigned char m_abReservedD0[0x68];
	int m_anRuntimeState138[4];
};

// GLOBAL: LEMBALL 0x004a7840
short g_LEVEL_MineTileVariantCodes[4];

void LEMBALL_FASTCALL ResetMineChunkObjectRuntimeState(MineChunkObjectRuntimeView* pObject);

// FUNCTION: LEMBALL 0x00423c10
MineChunkObjectView* MineChunkObjectView::ConstructMineChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x13, 0, 0);
	*(void**) this = g_LEVELVT_MineChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00423c30
void LEMBALL_FASTCALL ResetMineChunkObject(MineChunkObjectRuntimeView* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetMineChunkObjectRuntimeState(pObject);
}

// FUNCTION: LEMBALL 0x00423c50
void LEMBALL_FASTCALL ResetMineChunkObjectRuntimeState(MineChunkObjectRuntimeView* pObject)
{
	pObject->m_nStateB8 = 0x18;
	pObject->m_anRuntimeState138[0] = 0;
	pObject->m_anRuntimeState138[1] = 0;
	pObject->m_anRuntimeState138[2] = 0;
	pObject->m_anRuntimeState138[3] = 0;
	pObject->m_nFrameTickCC = g_nLevelFrameClockTick;
	g_LEVEL_MineTileVariantCodes[0] = 10;
	g_LEVEL_MineTileVariantCodes[1] = 0x11;
	g_LEVEL_MineTileVariantCodes[2] = 0x30;
	g_LEVEL_MineTileVariantCodes[3] = 8;
}

// FUNCTION: LEMBALL 0x00423dd0
void LEMBALL_FASTCALL SetMineOrCollTileVariant(void* pvObject)
{
	char* pObject;
	char* pGrid;
	MineChunkObjectNotifyVtable* pVtable;
	int x;
	int y;

	pObject = (char*) pvObject;
	x = (*(int*) (pObject + 0x9c) >> 12) / 16;
	y = (*(int*) (pObject + 0xa0) >> 12) / 16;
	if (*(int*) (pObject + 0x140) == 0) {
		pGrid = (char*) g_pLevelTileGrid;
		((LevelTileGridOwnerView*) pGrid)->SetCellType(x, y, 0x20a, g_LEVEL_MineTileVariantCodes[*(int*) pGrid]);
		*(int*) (pObject + 0x10) = 1;
		if (x >= 0 && y >= 0 && x < *(int*) (pGrid + 0x10) && y < *(int*) (pGrid + 0x14)) {
			*(unsigned char*) (*(char**) (pGrid + 0x0c) + (y * *(int*) (pGrid + 0x10) + x) * 12 + 6) |= 4;
		}
	}
	pVtable = *(MineChunkObjectNotifyVtable**) pObject;
	(((MineChunkObjectNotifyView*) pObject)->*pVtable->m_pNotify34)(0x10);
}

// FUNCTION: LEMBALL 0x00423eb0
int LEMBALL_FASTCALL AdvanceMineChunkObjectStateMachine(void* pvObject)
{
	char* pObject;
	int nState;

	pObject = (char*) pvObject;
	nState = *(int*) (pObject + 0xb8);
	if (*(int*) (pObject + 0x114) != 0) {
		if (*(int*) (pObject + 0x128) != nState) {
			switch (nState) {
			case 8:
				*(int*) (pObject + 0x13c) = 0;
				*(int*) (pObject + 0x138) = 0;
				break;
			case 0x1b:
				SetMineOrCollTileVariant(pObject);
				break;
			}
		}
		*(int*) (pObject + 0x128) = *(int*) (pObject + 0xb8);
		return 1;
	}
	switch (nState) {
	case 0x19:
		*(int*) (pObject + 0x140) = 0;
		break;
	case 0x1a:
		if ((unsigned int) *(int*) (pObject + 0xc8) < (unsigned int) g_nLevelFrameClockTick) {
			SetMineOrCollTileVariant(pObject);
			*(int*) (pObject + 0x94) = g_nLevelFrameClockTimeMs;
			*(int*) (pObject + 0xcc) = g_nLevelFrameClockTick + 0x14;
			((MineChunkObjectActionView*) pObject)->SetManagedEntityStateId(0x1b);
		}
		break;
	case 0x1b:
		if ((unsigned int) *(int*) (pObject + 0xcc) < (unsigned int) g_nLevelFrameClockTick) {
			*(int*) (pObject + 0x13c) = 0;
			*(int*) (pObject + 0x138) = 0;
			*(int*) (pObject + 0xc8) = g_nLevelFrameClockTick + 100;
			((MineChunkObjectActionView*) pObject)->SetManagedEntityStateId(8);
		}
		break;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00424c60
void* LEMBALL_FASTCALL DeleteMineChunkObjectArrayOrScalar(void* pObject, void*, unsigned int fDelete)
{
	char* pEntry;
	int cEntries;

	if ((fDelete & 2) != 0) {
		pEntry = (char*) pObject - 4;
		cEntries = *(int*) pEntry;
		pEntry = (char*) pObject + cEntries * 0x150;
		while (--cEntries >= 0) {
			pEntry -= 0x150;
			DestroyLevelChunkObjectBase24CF0(pEntry);
		}
		FreeVSMemBlock((char*) pObject - 4);
		return pObject;
	}
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}
