#include "AI/CLaser.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_LINKSCF_LasrChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetLasrChunkObjectRuntimeStateThunk(void* pObject);
extern void* g_pLevelTileGrid;
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTick;

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

struct ManagedEntitySlotOwnerView {
	void SetManagedEntitySlotId(unsigned short nSlotId);
};

struct CGround {
	short GetZThunk(int nLocalX, int nLocalY);
};

typedef int(LEMBALL_FASTCALL* LaserPointSlotProc)(void*, void*, const int*);
typedef void(LEMBALL_FASTCALL* LaserIntSlotProc)(void*, void*, int);

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

// FUNCTION: LEMBALL 0x00428ab0
int CLaser::CheckHits(void)
{
	void* pObject = this;
	int* pFields = (int*) pObject;
	int nX = pFields[0x27] >> 12;
	int nY = pFields[0x28] >> 12;
	int nZ = pFields[0x29] >> 12;
	int nStepX;
	int nStepY;
	int nSteps;
	void* pCandidate = 0;

	switch (pFields[0x19]) {
	case 0x1e:
	case 0x30:
		nStepX = 0x10;
		nStepY = 0;
		nX += 8;
		break;
	case 0x2f:
	case 0x31:
		nStepX = 0;
		nStepY = 0x10;
		nY += 8;
		break;
	default:
		return 0;
	}

	for (nSteps = 0; nSteps < 8; ++nSteps) {
		char* pGrid;
		char* pOwner;
		unsigned short nTerrain;

		nX += nStepX;
		nY += nStepY;
		pGrid = (char*) g_pLevelTileGrid;
		if (nX < 0 || nY < 0 || (nX >> 4) >= *(int*) (pGrid + 0x10) || (nY >> 4) >= *(int*) (pGrid + 0x14)) {
			nTerrain = 0;
		}
		else {
			CGround* pTile =
				(CGround*) (*(char**) (pGrid + 0x0c) + (((nY >> 4) * *(int*) (pGrid + 0x10)) + (nX >> 4)) * 12);
			nTerrain = (unsigned short) pTile->GetZThunk(nX & 15, nY & 15);
		}
		if ((int) nTerrain > nZ) {
			break;
		}

		pOwner = (char*) g_pActiveManagedEntityOwner;
		*(void**) (pOwner + 0x150) = 0;
		*(int*) (pOwner + 0x124) = nX;
		*(int*) (pOwner + 0x128) = nY;
		*(int*) (pOwner + 0x12c) = nZ;
		*(int*) (pOwner + 0x130) = 0;
		pCandidate = 0;
		while (*(int*) (pOwner + 0x130) < *(int*) (pOwner + 0x118)) {
			pCandidate = (*(void***) (pOwner + 0x120))[*(int*) (pOwner + 0x130)];
			if (pCandidate != *(void**) (pOwner + 0x150) &&
				((LaserPointSlotProc) (*(void***) pCandidate)[20])(pCandidate, 0, (int*) (pOwner + 0x124)) != 0) {
				++*(int*) (pOwner + 0x130);
				break;
			}
			pCandidate = 0;
			++*(int*) (pOwner + 0x130);
		}
		if (pCandidate != 0 && *(int*) ((char*) pCandidate + 0x64) == 2) {
			break;
		}
		pCandidate = 0;
	}

	if (pCandidate == 0) {
		return 0;
	}
	*(void**) ((char*) pObject + 0x144) = pCandidate;
	*(int*) ((char*) pCandidate + 0xb8) = 0x0f;
	*(short*) ((char*) pCandidate + 0xbc) = 1;
	*(int*) ((char*) pCandidate + 0xcc) = g_nLevelFrameClockTick + 0x1a;
	((LaserIntSlotProc) (*(void***) pObject)[13])(pObject, 0, 0x22);
	return 1;
}

void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject)
{
	((CLaser*) pObject)->~CLaser();
}
