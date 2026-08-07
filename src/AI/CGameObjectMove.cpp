#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern int g_nLevelFrameClockTick;
extern void* g_pLevelTileGrid;
extern void* g_pManagedEntityReachabilityHelper;
extern unsigned int __cdecl ReturnFacingDirection(int nX1, int nY1, int nX2, int nY2);
extern int Distance2DIntPixels(int x1, int y1, int x2, int y2);

// Minimal view-of the LEVELSTAT LevelTileGridOwnerView height sampler, so StartMoving
// can emit a direct `call LevelTileGridOwnerView::GetZ` (matches the orig ILT 0x401460 ->
// 0x4304e0). Method mangling depends only on name+signature, so this links to the
// LEVELSTAT definition at the same address regardless of the omitted field layout.
struct LevelTileGridOwnerView {
	unsigned short GetZ(int x, int y, void** ppMoveChunk);
};

// Macintosh: CGameObject::MapCheck(int, int)
// FUNCTION: LEMBALL 0x004157b0
int CGameObject::MapCheck(int nX, int nY)
{
	int iVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	unsigned short uVar5;
	int iVar6;

	iVar1 = (nY + (nY >> 31 & 0xf)) >> 4;
	iVar2 = nX + (nX >> 31 & 0xf);
	iVar3 = iVar2 >> 4;
	iVar4 = (unsigned int) (unsigned short) (iVar2 >> 0x14) << 0x10;
	iVar2 = iVar3;
	do {
		iVar6 = iVar1;
		if (iVar1 >= 0) {
			do {
				if (iVar2 < 0 || iVar6 < 0 || *(int*) ((char*) g_pLevelTileGrid + 0x10) <= iVar2 || *(int*) ((char*) g_pLevelTileGrid + 0x14) <= iVar6) {
					uVar5 = 3;
				}
				else {
					uVar5 = *(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar6 + iVar2) * 0xc);
				}
				iVar4 = ((unsigned int) (short) (iVar4 >> 0x10) << 0x10) | (unsigned short) (iVar4 | uVar5);
				iVar6 = iVar6 + 1;
			} while (iVar6 <= iVar1);
		}
		iVar2 = iVar2 + 1;
	} while (iVar2 <= iVar3);
	return iVar4;
}

// GLOBAL: LEMBALL 0x0049d020
static const int g_LEVEL_HeadingTurnDirections[8] = {0, 1, 1, 1, 1, -1, -1, -1};

// GLOBAL: LEMBALL 0x0049d0b0
static const int g_LEVEL_ManagedEntityUpdatePeriods[9] = {0, 87, 75, 0, 0, 0, 0, 75, 75};

struct LevelManagedEntityStateFields {
	char m_abReserved00[0xb4];
	short m_nHeadingOctantB4;

	void IncrementHeadingOctant(void);
	void DecrementHeadingOctant(void);
};

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation in LEVELVT.CPP.

// Macintosh: CGameObject::StartMoving()
// FUNCTION: LEMBALL 0x00415580
void CGameObject::StartMoving(void)
{
	void* pMoveChunk = 0;
	int nHeight;
	int nDestHeight;
	int nTickHeight;
	int nDist;
	int* pDest;
	int nSpeedIndex;
	int nPos[2];
	int nDest[2];

	if (m_pCommandQueue70 != 0) {
		nHeight = ((LevelTileGridOwnerView*) g_pLevelTileGrid)->GetZ(m_WorldPosition9C.x >> 12, m_WorldPosition9C.y >> 12, &pMoveChunk) & 0xffff;
		nDestHeight = m_WorldPosition9C.z >> 12;   /* 0xa4 */
		if (m_fOnMover11C == 0 && pMoveChunk != 0) {
			((void(__fastcall*)(void*, void*)) 0x4036b1)(pMoveChunk, this);
		}
		if (nDestHeight == nHeight) {
			pDest = (int*) ((void* (__fastcall*)(CGameObject*, int*)) 0x40336e)(this, &nHeight);
			nPos[0] = pDest[0];
			nPos[1] = pDest[1];
			*(int*) ((char*) this + 0xb0) = pDest[2];
			nDist = Distance2DIntPixels(m_WorldPosition9C.x >> 12, m_WorldPosition9C.y >> 12, nPos[0] >> 12, nPos[1] >> 12);
			m_nMotionStartTickC8 = g_nLevelFrameClockTick;
			nSpeedIndex = *((int*) ((char*) this + 0x64));
			m_nMotionDuration88 = (*(int*) (0x49d070 + nSpeedIndex * 4) * nDist) / 0x32;
			if (m_nMotionDuration88 == 0) {
				m_nMotionDuration88 = 1;
			}
			nPos[0] = m_WorldPosition9C.x;
			nPos[1] = m_WorldPosition9C.y;
			m_nNextUpdateTickCC = m_nMotionDuration88 + g_nLevelFrameClockTick;
			nDest[0] = *(int*) ((char*) this + 0xa8);
			nDest[1] = *(int*) ((char*) this + 0xac);
			((void(__fastcall*)(int*, int*)) 0x40119a)(nDest, nDest);
			((void(__fastcall*)(int*, int*)) 0x40119a)(nPos, nPos);
			((void(__fastcall*)(void*, int, void*, int, int)) 0x40326f)((char*) this + 0x78, 0x4156c1, nDest, nPos[0] >> 12, nPos[1] >> 12);
			return;
		}
		if (*(int*) ((char*) this + 0x30) == 0) {
			nTickHeight = g_nLevelFrameClockTick;
			if ((*(unsigned char*) ((char*) this + 0x68) & 4) != 0) {
				*(int*) ((char*) this + 0x50) = 0;
				*(int*) ((char*) this + 0x108) = 1;
				*(int*) ((char*) this + 0x4c) = 0x3000;
				*(int*) ((char*) this + 0x54) = ((((nDestHeight - nHeight) + ((nDestHeight - nHeight) >> 31 & 7)) >> 3) + 1) * 0x1000;
				m_nMotionStartTickC8 = g_nLevelFrameClockTick;
				*(short*) ((char*) this + 0xbc) = 0;
				*(int*) ((char*) this + 0x100) = nDestHeight;
				*(int*) ((char*) this + 0xf4) = m_WorldPosition9C.x;
				*(int*) ((char*) this + 0xf8) = m_WorldPosition9C.y;
				*(int*) ((char*) this + 0xfc) = nHeight << 12;
				return;
			}
		}
		else {
			m_WorldPosition9C.z = nHeight << 12;
		}
	}
}
void CGameObject::StopMoving(void)
{
	int nFrameClockTick;
	DeleteFirstEntryFromDestinationList();
	m_nMotionDuration88 = 0;
	nFrameClockTick = g_nLevelFrameClockTick;
	m_nNextUpdateTickCC = nFrameClockTick;
	m_nMotionStartTickC8 = nFrameClockTick;
}

// Macintosh: CGameObject::TurnToFaceDestination()
// FUNCTION: LEMBALL 0x00415d90
void CGameObject::TurnToFaceDestination(void)
{
	AICOORD targetPosition;
	int nDirection;

	GetDestination(&targetPosition);
	nDirection = ReturnFacingDirection(m_WorldPosition9C.x >> 12,
														 m_WorldPosition9C.y >> 12,
														 targetPosition.x >> 12,
														 targetPosition.y >> 12);
	if (nDirection != m_nHeadingOctantB4) {
		LevelManagedEntityStateFields* pState = (LevelManagedEntityStateFields*) this;
		if (g_LEVEL_HeadingTurnDirections[(nDirection - m_nHeadingOctantB4) & 7] < 0) {
			pState->DecrementHeadingOctant();
		}
		else {
			pState->IncrementHeadingOctant();
		}
	}
	m_nNextUpdateTickCC = g_nLevelFrameClockTick + g_LEVEL_ManagedEntityUpdatePeriods[m_nEntityType64] / 50;
}

// Macintosh: CGameObject::FacingDestination()
// FUNCTION: LEMBALL 0x00415e20
int CGameObject::FacingDestination(void)
{
	AICOORD targetPosition;
	int nDirection;

	GetDestination(&targetPosition);
	nDirection = ReturnFacingDirection(m_WorldPosition9C.x >> 12,
														 m_WorldPosition9C.y >> 12,
														 targetPosition.x >> 12,
														 targetPosition.y >> 12);
	return m_nHeadingOctantB4 == nDirection;
}

// Macintosh: CGameObject::DeleteFirstEntryFromDestinationList()
// FUNCTION: LEMBALL 0x00415e80
void CGameObject::DeleteFirstEntryFromDestinationList(void)
{
	CGameObjectCommand* pDestination;
	CGameObjectCommand* pSource;
	CGameObjectCommandQueue* pQueue;
	int iEntry;
	int nOffset;

	pQueue = m_pCommandQueue70;
	nOffset = 0;
	if (pQueue->m_cEntries != 0) {
		iEntry = 0;
		if (pQueue->m_cEntries - 1 > 0) {
			do {
				pDestination = (CGameObjectCommand*) ((char*) pQueue->m_pEntries + nOffset);
				++iEntry;
				nOffset += sizeof(CGameObjectCommand);
				pSource = pDestination + 1;
				pDestination->m_nType = pSource->m_nType;
				pDestination->m_Position.x = pSource->m_Position.x;
				pDestination->m_Position.y = pSource->m_Position.y;
				pDestination->m_Position.z = pSource->m_Position.z;
				pDestination->m_nFlags = pSource->m_nFlags;
			} while (iEntry < pQueue->m_cEntries - 1);
		}
		--pQueue->m_cEntries;
	}
	m_fHasCommands74 = 0 < pQueue->m_cEntries;
}

// Macintosh: CGameObject::AddDestination(const AICOORD&)
// FUNCTION: LEMBALL 0x00415ef0
void CGameObject::AddDestination(const AICOORD& position)
{
	CGameObjectCommandQueue* pQueue;
	CGameObjectCommand* pCommand;
	unsigned short iEntry;

	pQueue = m_pCommandQueue70;
	if (pQueue != 0 && pQueue->m_cEntries < pQueue->m_cCapacity) {
		iEntry = pQueue->m_cEntries++;
		pCommand = &pQueue->m_pEntries[iEntry];
		pCommand->m_nType = 1;
		pCommand->m_Position.x = position.x;
		pCommand->m_Position.y = position.y;
		pCommand->m_Position.z = position.z;
	}
}

// Macintosh: CGameObject::GetDestination()
// FUNCTION: LEMBALL 0x00416000
AICOORD* CGameObject::GetDestination(AICOORD* pPosition)
{
	if (m_pCommandQueue70->m_cEntries > 0) {
		CGameObjectCommand* pCommand = m_pCommandQueue70->m_pEntries;
		pPosition->x = pCommand->m_Position.x;
		pPosition->y = pCommand->m_Position.y;
		pPosition->z = pCommand->m_Position.z;
	}
	else {
		pPosition->x = m_WorldPosition9C.x;
		pPosition->y = m_WorldPosition9C.y;
		pPosition->z = m_WorldPosition9C.z;
	}
	return pPosition;
}

// FUNCTION: LEMBALL 0x004160c0
int CGameObject::DestinationExists(void)
{
	return 0 < m_pCommandQueue70->m_cEntries;
}

// FUNCTION: LEMBALL 0x004160e0
void CGameObject::EmptyDestinationList(void)
{
	m_pCommandQueue70->m_cEntries = 0;
}

// FUNCTION: LEMBALL 0x004160f0
void CGameObject::GetBoundingBox(CVSRect& rect)
{
	rect.x = (m_WorldPosition9C.x >> 12) - 24;
	rect.y = (m_WorldPosition9C.y >> 12) - 24;
	rect.width = 48;
	rect.height = 48;
}

// FUNCTION: LEMBALL 0x00416570
int CGameObject::IsSelectable(void)
{
	int nState = m_nStateB8;
	if (nState >= 7) {
		if (nState <= 8 || nState == 0x15) {
			goto inactive;
		}
	}
	return 1;
inactive:
	return 0;
}

typedef int(LEMBALL_FASTCALL* CGameObjectInterruptibleProc)(CGameObject* pObject);

// FUNCTION: LEMBALL 0x00416590
void CGameObject::ResetInstructions(void)
{
	if (m_nStateB8 != 4 && m_nStateB8 != 0xc && m_pCommandQueue70 != 0) {
		CGameObjectInterruptibleProc pIsInterruptible = (CGameObjectInterruptibleProc) ((void**) m_pVtable00)[0x1c];
		if (pIsInterruptible(this) != 0) {
			m_nNextUpdateTickCC = g_nLevelFrameClockTick;
		}
		m_pCommandQueue70->m_cEntries = 0;
		if (m_pReachabilityOwner0C != 0) {
			*(int*) ((char*) g_pManagedEntityReachabilityHelper + 8) = 0;
		}
		m_nPendingInstruction08 = 0;
	}
}

// FUNCTION: LEMBALL 0x00419ea0
void __cdecl ThunkPopFirstManagedEntityMoveCommandAndResetMotion(void* pUnused, CGameObject* pEntity)
{
	(void) pUnused;
	pEntity->StopMoving();
}

// FUNCTION: LEMBALL 0x00419ec0
void __cdecl ThunkUpdateManagedEntityFacingTowardFirstMoveCommand(void* pUnused, CGameObject* pEntity)
{
	(void) pUnused;
	pEntity->TurnToFaceDestination();
}
