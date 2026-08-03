#include "AI/CMine.h"

#include "AI/AICoord.h"
#include "AI/CMineManager.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Mixed/Level/CHUNKOBJVT.H"
#include "Visos/Generic/Memory.h"

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct ManagedEntitySlotOwnerView {
	void SetManagedEntitySlotId(unsigned short nSlotId);
};

struct CGround {
	short GetZ(int nLocalX, int nLocalY);
};

struct MineChunkObjectActionView {
	virtual void Reserved0(void);
	virtual void Reserved1(void);
	virtual void SetManagedEntityStateId(int nStateId);
};

struct MineChunkObjectNotifyView {};
typedef void (MineChunkObjectNotifyView::*MineNotifyProc)(int nValue);
typedef void(LEMBALL_FASTCALL* MineContactNotifyProc)(void* pObject);
struct MineChunkObjectNotifyVtable {
	void* m_apReserved00[13];
	MineNotifyProc m_pNotify34;
};

struct LevelTileGridOwnerView {
	void SetTerrain(int x, int y, int nType, int nVariant);
};

extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTimeMs;
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL destroy_level_chunk_object_base24cf0(void* pObject);
void* LEMBALL_FASTCALL DeleteMineChunkObjectArrayOrScalar(void* pObject, void* pUnused, unsigned int fDelete);

struct CMineVtable {
	void* m_pDelete00;
	void* m_pSetVariant04;
	void* m_pSetState08;
	void* m_pRender0C;
	void* m_pNoop10;
	int (CMine::*m_pProcess14)(void);
	void* m_pSlot618;
	void* m_pGetF01C;
	void* m_pSetMotion20;
	void* m_pSlot924;
	void* m_pSlot1028;
	void* m_pSlot112C;
	void* m_pNoop30;
	void* m_pSlot1334;
	void* m_pGet9838;
	void* m_pSlot153C;
};

// GLOBAL: LEMBALL 0x00495be8
CMineVtable g_LEVELVT_MineChunkObjectVtable = {(void*) DeleteMineChunkObjectArrayOrScalar,
											   (void*) SetManagedEntityStateIdWithVariant,
											   (void*) SetManagedEntityStateId,
											   (void*) EmitLevelChunkObjectRenderEntry,
											   (void*) noop_vtable_callback_0040a7f0,
											   &CMine::Process,
											   (void*) return_true_vtable_callback_0040a800,
											   (void*) get_object_field_0xf0,
											   (void*) SetLevelChunkObjectMotionVector,
											   (void*) AdvanceManagedEntityGravityMotion,
											   (void*) return_false_vtable_callback_0040a820,
											   (void*) copy_object_fields_0x9c_triplet,
											   (void*) noop_vtable_callback_0040a860,
											   (void*) set_object_field_0x98,
											   (void*) get_object_field_0x98,
											   (void*) BeginManagedEntityReachablePathSearch};

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

// FUNCTION: LEMBALL 0x00423c10
CMine::CMine(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x13, 0, 0);
	*(void**) this = &g_LEVELVT_MineChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x00423c30
void CMine::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	Initialise();
}

// FUNCTION: LEMBALL 0x00423c50
void CMine::Initialise(void)
{
	MineChunkObjectRuntimeView* pObject = (MineChunkObjectRuntimeView*) this;

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

// FUNCTION: LEMBALL 0x00423cb0
void CMine::Set(AICOORD position)
{
	char* pGrid;
	int x;
	int y;

	*(int*) ((char*) this + 0x9c) = position.x;
	*(int*) ((char*) this + 0xa0) = position.y;
	*(int*) ((char*) this + 0xa4) = position.z;
	*(int*) ((char*) this + 0x13c) = 0;
	*(int*) ((char*) this + 0x138) = 1;
	*(int*) ((char*) this + 0x140) = 0;
	x = (position.x >> 12) / 16;
	y = (position.y >> 12) / 16;
	if (x >= 0 && y >= 0) {
		pGrid = (char*) g_pLevelTileGrid;
		if (x < *(int*) (pGrid + 0x10) && y < *(int*) (pGrid + 0x14)) {
			*(unsigned char*) (*(char**) (pGrid + 0x0c) + (y * *(int*) (pGrid + 0x10) + x) * 12 + 7) |= 0x80;
		}
	}
}

// FUNCTION: LEMBALL 0x00423d40
void CMine::Trigger(int nDelay)
{
	if (*(int*) ((char*) this + 0x144) == 0 && *(int*) ((char*) this + 0xb8) == 0x18) {
		*(int*) ((char*) this + 0x144) = 1;
		*(int*) ((char*) this + 0x148) = nDelay;
		((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
	}
}

// FUNCTION: LEMBALL 0x00423d70
void CMine::DoActivate(void)
{
	*(int*) ((char*) this + 0x13c) = 1;
	if (*(int*) ((char*) this + 0x144) != 0) {
		*(int*) ((char*) this + 0x140) = 0;
		*(int*) ((char*) this + 0xc8) = *(int*) ((char*) this + 0x148) + g_nLevelFrameClockTick;
		((CMineManager*) *(void**) ((char*) this + 0x60))->Triggered(this);
		return;
	}
	SetTerrain();
	*(int*) ((char*) this + 0x94) = g_nLevelFrameClockTimeMs;
	*(int*) ((char*) this + 0xcc) = g_nLevelFrameClockTick + 0x14;
}

// FUNCTION: LEMBALL 0x00423dd0
void CMine::SetTerrain(void)
{
	char* pObject;
	char* pGrid;
	MineChunkObjectNotifyVtable* pVtable;
	int x;
	int y;

	pObject = (char*) this;
	x = (*(int*) (pObject + 0x9c) >> 12) / 16;
	y = (*(int*) (pObject + 0xa0) >> 12) / 16;
	if (*(int*) (pObject + 0x140) == 0) {
		pGrid = (char*) g_pLevelTileGrid;
		((LevelTileGridOwnerView*) pGrid)->SetTerrain(x, y, 0x20a, g_LEVEL_MineTileVariantCodes[*(int*) pGrid]);
		*(int*) (pObject + 0x10) = 1;
		if (x >= 0 && y >= 0 && x < *(int*) (pGrid + 0x10) && y < *(int*) (pGrid + 0x14)) {
			*(unsigned char*) (*(char**) (pGrid + 0x0c) + (y * *(int*) (pGrid + 0x10) + x) * 12 + 6) |= 4;
		}
	}
	pVtable = *(MineChunkObjectNotifyVtable**) pObject;
	(((MineChunkObjectNotifyView*) pObject)->*pVtable->m_pNotify34)(0x10);
}

// FUNCTION: LEMBALL 0x00423e70
void CMine::StepOn(CGameObject* pObject)
{
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1b);
	((MineContactNotifyProc) (*(void***) pObject)[25])(pObject);
}

// FUNCTION: LEMBALL 0x00423e90
int CMine::IsUsable(eAction action)
{
	return action == 8 || action == 0x18;
}

// FUNCTION: LEMBALL 0x00423eb0
int CMine::Process(void)
{
	char* pObject;
	int nState;

	pObject = (char*) this;
	nState = *(int*) (pObject + 0xb8);
	if (*(int*) (pObject + 0x114) != 0) {
		if (*(int*) (pObject + 0x128) != nState) {
			switch (nState) {
			case 8:
				*(int*) (pObject + 0x13c) = 0;
				*(int*) (pObject + 0x138) = 0;
				break;
			case 0x1b:
				((CMine*) pObject)->SetTerrain();
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
			((CMine*) pObject)->SetTerrain();
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

// FUNCTION: LEMBALL 0x00423fa0
void CMine::OnGround(void)
{
	char* pGrid;
	int x;
	int y;
	short z;

	x = *(int*) ((char*) this + 0x9c) >> 12;
	y = *(int*) ((char*) this + 0xa0) >> 12;
	pGrid = (char*) g_pLevelTileGrid;
	if (x < 0 || y < 0 || x / 16 >= *(int*) (pGrid + 0x10) || y / 16 >= *(int*) (pGrid + 0x14)) {
		z = 0;
	}
	else {
		z = ((CGround*) (*(char**) (pGrid + 0x0c) + (y / 16 * *(int*) (pGrid + 0x10) + x / 16) * 12))
				->GetZ(x & 15, y & 15);
	}
	*(int*) ((char*) this + 0xa4) = (unsigned short) z << 12;
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
			destroy_level_chunk_object_base24cf0(pEntry);
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
