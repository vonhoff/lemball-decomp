#include "AI/CLaser.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_LINKSCF_LasrChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetLasrChunkObjectRuntimeStateThunk(void* pObject);
extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTick;

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

struct ManagedEntitySlotOwnerView {
	void SetManagedEntitySlotId(unsigned short nSlotId);
};

// FUNCTION: LEMBALL 0x00428890
CLaser::CLaser(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x2f, 0, 0);
	*(void**) this = g_LINKSCF_LasrChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x004288b0
void CLaser::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	ResetLasrChunkObjectRuntimeStateThunk(this);
}

// FUNCTION: LEMBALL 0x004288d0
void CLaser::Initialise(void)
{
	char* pObjectBytes;
	int nZero;

	pObjectBytes = (char*) this;
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	nZero = 0;
	*(int*) (pObjectBytes + 0x94) = nZero;
	*(int*) (pObjectBytes + 0x138) = nZero;
	*(int*) (pObjectBytes + 0x13c) = nZero;
}

// FUNCTION: LEMBALL 0x004288f0
CLaser::~CLaser(void)
{
	*(void**) this = g_LINKSCF_LasrChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x00428900
void CLaser::Set(unsigned short nSlotId, const AICOORD& position, int nObjectType)
{
	char* pObjectBytes;
	char* pGrid;
	int nTileX;
	int nTileY;
	int i;
	int x;
	int y;

	pObjectBytes = (char*) this;
	((ManagedEntitySlotOwnerView*) this)->SetManagedEntitySlotId(nSlotId);
	*(int*) (pObjectBytes + 0x9c) = position.x;
	nTileX = (position.x >> 12) / 16;
	*(int*) (pObjectBytes + 0xa0) = position.y;
	*(int*) (pObjectBytes + 0xa4) = position.z;
	*(int*) (pObjectBytes + 0x64) = nObjectType;
	*(int*) (pObjectBytes + 0x13c) = 1;
	nTileY = (position.y >> 12) / 16;

	switch (nObjectType) {
	case 0x1e:
		*(int*) (pObjectBytes + 0xb8) = 0x18;
		*(int*) (pObjectBytes + 0x140) = 1;
		*(int*) (pObjectBytes + 0x138) = 1;
		break;
	case 0x2f:
		*(int*) (pObjectBytes + 0xb8) = 0x18;
		*(int*) (pObjectBytes + 0x140) = 1;
		*(int*) (pObjectBytes + 0x138) = 1;
		break;
	case 0x30:
		*(int*) (pObjectBytes + 0x140) = 0;
		*(int*) (pObjectBytes + 0xb8) = 0x18;
		*(int*) (pObjectBytes + 0x138) = 1;
		for (i = 1; i < 8; ++i) {
			x = nTileX + i;
			if (x >= 0 && nTileY >= 0) {
				pGrid = (char*) g_pLevelTileGrid;
				if (x < *(int*) (pGrid + 0x10) && nTileY < *(int*) (pGrid + 0x14)) {
					*(unsigned char*) (*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + x) * 12 + 7) |=
						0x80;
				}
			}
		}
		break;
	case 0x31:
		*(int*) (pObjectBytes + 0x140) = 0;
		*(int*) (pObjectBytes + 0xb8) = 0x18;
		*(int*) (pObjectBytes + 0x138) = 1;
		for (i = 1; i < 8; ++i) {
			y = nTileY + i;
			if (nTileX >= 0 && y >= 0) {
				pGrid = (char*) g_pLevelTileGrid;
				if (nTileX < *(int*) (pGrid + 0x10) && y < *(int*) (pGrid + 0x14)) {
					*(unsigned char*) (*(char**) (pGrid + 0x0c) + (y * *(int*) (pGrid + 0x10) + nTileX) * 12 + 7) |=
						0x80;
				}
			}
		}
		break;
	}
	*(int*) (pObjectBytes + 0xcc) = g_nLevelFrameClockTick + 0x3c;
}

void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject)
{
	((CLaser*) pObject)->~CLaser();
}
