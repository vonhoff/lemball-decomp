#include "AI/CLaser.h"

#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

struct LaserVtableLayout;
extern LaserVtableLayout g_LINKSCF_LasrChunkObjectVtable;
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetLasrChunkObjectRuntimeStateThunk(void* pObject);
extern void* g_pLevelTileGrid;
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern int g_nSelectedNetworkLobbyPeerId;
extern int Distance2DIntPixels(int x1, int y1, int x2, int y2);
extern void LEMBALL_FASTCALL EmitLevelChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry);

struct CLaserTileGrid {
	char m_abReserved00[0xc];
	unsigned char* m_pTileData0C;   // 0x0c
	int m_nWidth10;                  // 0x10
	int m_nHeight14;                 // 0x14
};

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

struct CGround {
	short GetZThunk(int nLocalX, int nLocalY);
};

typedef int(LEMBALL_FASTCALL* LaserPointSlotProc)(void*, void*, const int*);
typedef void(LEMBALL_FASTCALL* LaserIntSlotProc)(void*, void*, int);

struct LaserStateView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateId(int nStateId);
};

// FUNCTION: LEMBALL 0x00428890
CLaser::CLaser(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x2f, 0, 0);
	*(void**) this = &g_LINKSCF_LasrChunkObjectVtable;
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
	m_nStateB8 = 0x18;
	nZero = 0;
	*(int*) (pObjectBytes + 0x94) = nZero;
	m_anRuntimeState138[0] = nZero;
	m_anRuntimeState138[1] = nZero;
}

// FUNCTION: LEMBALL 0x004288f0
CLaser::~CLaser(void)
{
	*(void**) this = &g_LINKSCF_LasrChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x00428900
void CLaser::Set(unsigned short nSlotId, const AICOORD& position, int nObjectType)
{
	char* pObjectBytes;
	int nTileX;
	int nTileY;
	int i;
	int x;
	int y;

	pObjectBytes = (char*) this;
	((CGameObject*) this)->SetId(nSlotId);
	m_WorldPosition9C.x = position.x;
	nTileX = (position.x >> 12) / 16;
	m_WorldPosition9C.y = position.y;
	m_WorldPosition9C.z = position.z;
	m_nEntityType64 = nObjectType;
	m_anRuntimeState138[1] = 1;
	nTileY = (position.y >> 12) / 16;

	switch (nObjectType) {
	case 0x1e:
		m_nStateB8 = 0x18;
		m_anRuntimeState138[2] = 1;
		m_anRuntimeState138[0] = 1;
		break;
	case 0x2f:
		m_nStateB8 = 0x18;
		m_anRuntimeState138[2] = 1;
		m_anRuntimeState138[0] = 1;
		break;
	case 0x30:
		m_anRuntimeState138[2] = 0;
		m_nStateB8 = 0x18;
		m_anRuntimeState138[0] = 1;
		for (i = 1; i < 8; ++i) {
			x = nTileX + i;
			if (x >= 0 && nTileY >= 0) {
				CLaserTileGrid* pGrid = (CLaserTileGrid*) g_pLevelTileGrid;
				if (x < pGrid->m_nWidth10 && nTileY < pGrid->m_nHeight14) {
					pGrid->m_pTileData0C[(nTileY * pGrid->m_nWidth10 + x) * 12 + 7] |= 0x80;
				}
			}
		}
		break;
	case 0x31:
		m_anRuntimeState138[2] = 0;
		m_nStateB8 = 0x18;
		m_anRuntimeState138[0] = 1;
		for (i = 1; i < 8; ++i) {
			y = nTileY + i;
			if (nTileX >= 0 && y >= 0) {
				CLaserTileGrid* pGrid = (CLaserTileGrid*) g_pLevelTileGrid;
				if (nTileX < pGrid->m_nWidth10 && y < pGrid->m_nHeight14) {
					pGrid->m_pTileData0C[(y * pGrid->m_nWidth10 + nTileX) * 12 + 7] |= 0x80;
				}
			}
		}
		break;
	}
	m_nFrameTickCC = g_nLevelFrameClockTick + 0x3c;
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
		char* pOwner;
		unsigned short nTerrain;

		nX += nStepX;
		nY += nStepY;
		CLaserTileGrid* pGrid = (CLaserTileGrid*) g_pLevelTileGrid;
		if (nX < 0 || nY < 0 || (nX >> 4) >= pGrid->m_nWidth10 || (nY >> 4) >= pGrid->m_nHeight14) {
			nTerrain = 0;
		}
		else {
			CGround* pTile =
				(CGround*) (pGrid->m_pTileData0C + (((nY >> 4) * pGrid->m_nWidth10) + (nX >> 4)) * 12);
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

// FUNCTION: LEMBALL 0x00428cf0
int CLaser::Process(void)
{
	void* pObject = this;
	int* pFields = (int*) pObject;
	void* pTarget;
	int nState;

	if (pFields[0x45] != 0) {
		pFields[0x4e] = pFields[0x2e] != 0x18;
		if (pFields[0x2e] == 0x1a && *(void**) ((char*) pObject + 0x144) == 0) {
			CheckHits();
		}
		nState = pFields[0x2e];
		if (pFields[0x4a] != nState) {
			if (nState == 0x17) {
				pTarget = *(void**) ((char*) pObject + 0x144);
				if (pTarget != 0) {
					*(int*) ((char*) pTarget + 0x2c) = 1;
					*(void**) ((char*) pObject + 0x144) = 0;
				}
				((LaserStateView*) pObject)->SetManagedEntityStateId(0x18);
			}
			else if (nState == 0x19) {
				*(void**) ((char*) pObject + 0x144) = 0;
			}
			pFields[0x4a] = pFields[0x2e];
		}
		return 1;
	}

	if (g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) != 0) {
		return 1;
	}
	if (pFields[0x4e] == 0) {
		return 1;
	}

	switch (pFields[0x2e]) {
	case 0x17:
		pTarget = *(void**) ((char*) pObject + 0x144);
		if (pTarget != 0) {
			((LaserIntSlotProc) (*(void***) pTarget)[13])(pTarget, 0, 0x22);
			*(int*) ((char*) pTarget + 0x2c) = 1;
			*(void**) ((char*) pObject + 0x144) = 0;
		}
		((LaserStateView*) pObject)->SetManagedEntityStateId(0x18);
		return 1;
	case 0x18:
		if ((unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			Activate();
		}
		return 1;
	case 0x19:
		if ((unsigned int) pFields[0x34] < (unsigned int) g_nLevelFrameClockTick) {
			*(void**) ((char*) pObject + 0x144) = 0;
			((LaserStateView*) pObject)->SetManagedEntityStateId(0x1a);
		}
		return 1;
	case 0x1a:
		if (*(void**) ((char*) pObject + 0x144) == 0) {
			CheckHits();
		}
		if ((unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			pFields[0x4f] = 1;
			pFields[0x4e] = pFields[0x50];
			pFields[0x33] = g_nLevelFrameClockTick + 0x3c;
			pTarget = *(void**) ((char*) pObject + 0x144);
			if (pTarget != 0) {
				*(int*) ((char*) pTarget + 0x2c) = 1;
				*(void**) ((char*) pObject + 0x144) = 0;
			}
			((LaserStateView*) pObject)->SetManagedEntityStateId(0x17);
		}
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00428ec0
int CLaser::Activate(void)
{
	void* pObject = this;
	int* pFields = (int*) pObject;

	pFields[0x4e] = 1;
	if (g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) != 0) {
		return 0;
	}
	pFields[0x32] = g_nLevelFrameClockTick;
	pFields[0x34] = g_nLevelFrameClockTick + 6;
	pFields[0x33] = g_nLevelFrameClockTick + 0x18;
	*(void**) ((char*) pObject + 0x144) = 0;
	pFields[0x25] = g_nLevelFrameClockTimeMs;
	((LaserStateView*) pObject)->SetManagedEntityStateId(0x19);
	return 1;
}

// FUNCTION: LEMBALL 0x00428f30
int CLaser::StepOn(const AICOORD& position, CGameObject* pObject)
{
	char* pObjectBytes = (char*) pObject;
	int* pFields = (int*) this;

	if (m_nEntityType64 == 2 &&
		Distance2DIntPixels(pFields[0x27] >> 12, pFields[0x28] >> 12, position.x >> 12, position.y >> 12) < 0x30) {
		Activate();
		*(CGameObject**) ((char*) this + 0x144) = pObject;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00428f90
int CLaser::GetViewData(CViewData* pViewData)
{
	char* pObject;
	char* pEntry;
	CGround* pTile;
	unsigned short nTerrain;
	int nX;
	int nY;
	int nZ;
	int nStepX;
	int nStepY;
	int nRenderType;
	int cEntries;

	pObject = (char*) this;
	pEntry = (char*) pViewData;
	EmitLevelChunkObjectRenderEntry(this, 0, pEntry);
	cEntries = 1;
	if (m_nStateB8 != 0x1a) {
		return cEntries;
	}

	switch (*(int*) (pObject + 0x64)) {
	case 0x1e:
	case 0x30:
		nX = (*(int*) (pObject + 0x9c) >> 12) + 0x26;
		nY = *(int*) (pObject + 0xa0) >> 12;
		nStepX = 0x10;
		nStepY = 0;
		nRenderType = 0x32;
		break;
	case 0x2f:
	case 0x31:
		nX = *(int*) (pObject + 0x9c) >> 12;
		nY = (*(int*) (pObject + 0xa0) >> 12) + 0x14;
		nStepX = 0;
		nStepY = 0x10;
		nRenderType = 0x26;
		break;
	default:
		return cEntries;
	}
	nZ = (*(int*) (pObject + 0xa4) >> 12) + 3;
	pEntry += 0x4c;
	while (cEntries < 8) {
		CLaserTileGrid* pGrid = (CLaserTileGrid*) g_pLevelTileGrid;
		if (nX < 0 || nY < 0 || (nX >> 4) >= pGrid->m_nWidth10 || (nY >> 4) >= pGrid->m_nHeight14) {
			nTerrain = 0;
		}
		else {
			pTile = (CGround*) (pGrid->m_pTileData0C + (((nY >> 4) * pGrid->m_nWidth10) + (nX >> 4)) * 12);
			nTerrain = (unsigned short) pTile->GetZThunk(nX & 15, nY & 15);
		}
		if (nZ < (int) nTerrain) {
			return cEntries;
		}
		EmitLevelChunkObjectRenderEntry(this, 0, pEntry);
		*(unsigned short*) pEntry = 0;
		*(int*) (pEntry + 4) = nX;
		*(int*) (pEntry + 8) = nY;
		*(int*) (pEntry + 12) = nZ;
		*(int*) (pEntry + 0x18) = 0x1a;
		*(int*) (pEntry + 0x28) = nRenderType;
		++cEntries;
		nX += nStepX;
		nY += nStepY;
		pEntry += 0x4c;
	}
	return cEntries;
}

void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject)
{
	((CLaser*) pObject)->~CLaser();
}
