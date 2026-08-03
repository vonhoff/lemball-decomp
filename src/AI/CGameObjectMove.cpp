#include "AI/AICoord.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern int g_nLevelFrameClockTick;
extern unsigned int __cdecl compute_direction_octant_between_points(int nX1, int nY1, int nX2, int nY2);

// GLOBAL: LEMBALL 0x0049d020
static const int g_LEVEL_HeadingTurnDirections[8] = {0, 1, 1, 1, 1, -1, -1, -1};

// GLOBAL: LEMBALL 0x0049d0b0
static const int g_LEVEL_ManagedEntityUpdatePeriods[9] = {0, 87, 75, 0, 0, 0, 0, 75, 75};

struct LevelManagedEntityCommandEntry {
	int m_nField00;
	int m_nField04;
	int m_nField08;
	int m_nField0C;
	unsigned short m_nField10;
	unsigned short m_nReserved12;
};

struct LevelManagedEntityCommandQueue {
	unsigned short m_cEntries00;
	unsigned short m_cCapacity02;
	LevelManagedEntityCommandEntry* m_pEntries04;
};

struct CGameObject {
	char m_abReserved00[0x64];
	int m_nEntityType64;
	char m_abReserved68[8];
	LevelManagedEntityCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	char m_abReserved78[0x10];
	int m_nMotionDuration88;
	char m_abReserved8C[0x10];
	AICOORD m_WorldPosition9C;
	char m_abReservedA8[0xc];
	short m_nHeadingOctantB4;
	char m_abReservedB6[0x12];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;

	void DeleteFirstEntryFromDestinationList(void);
	void StopMoving(void);
	void TurnToFaceDestination(void);
	int FacingDestination(void);
	void AddDestination(const AICOORD& position);
	AICOORD* GetDestination(AICOORD* pPosition);
};

struct LevelManagedEntityStateFields {
	char m_abReserved00[0xb4];
	short m_nHeadingOctantB4;

	void IncrementHeadingOctant(void);
	void DecrementHeadingOctant(void);
};

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation in LEVELVT.CPP.

// Macintosh: CGameObject::StopMoving()
// FUNCTION: LEMBALL 0x00415780
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
	nDirection = compute_direction_octant_between_points(m_WorldPosition9C.x >> 12,
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
	nDirection = compute_direction_octant_between_points(m_WorldPosition9C.x >> 12,
														 m_WorldPosition9C.y >> 12,
														 targetPosition.x >> 12,
														 targetPosition.y >> 12);
	return m_nHeadingOctantB4 == nDirection;
}

// Macintosh: CGameObject::DeleteFirstEntryFromDestinationList()
// FUNCTION: LEMBALL 0x00415e80
void CGameObject::DeleteFirstEntryFromDestinationList(void)
{
	LevelManagedEntityCommandEntry* pDestination;
	LevelManagedEntityCommandEntry* pSource;
	LevelManagedEntityCommandQueue* pQueue;
	int iEntry;
	int nOffset;

	pQueue = m_pCommandQueue70;
	nOffset = 0;
	if (pQueue->m_cEntries00 != 0) {
		iEntry = 0;
		if (pQueue->m_cEntries00 - 1 > 0) {
			do {
				pDestination = (LevelManagedEntityCommandEntry*) ((char*) pQueue->m_pEntries04 + nOffset);
				++iEntry;
				nOffset += sizeof(LevelManagedEntityCommandEntry);
				pSource = pDestination + 1;
				pDestination->m_nField00 = pSource->m_nField00;
				pDestination->m_nField04 = pSource->m_nField04;
				pDestination->m_nField08 = pSource->m_nField08;
				pDestination->m_nField0C = pSource->m_nField0C;
				pDestination->m_nField10 = pSource->m_nField10;
			} while (iEntry < pQueue->m_cEntries00 - 1);
		}
		--pQueue->m_cEntries00;
	}
	m_fHasCommands74 = 0 < pQueue->m_cEntries00;
}

// Macintosh: CGameObject::AddDestination(const AICOORD&)
// FUNCTION: LEMBALL 0x00415ef0
void CGameObject::AddDestination(const AICOORD& position)
{
	LevelManagedEntityCommandQueue* pQueue;
	LevelManagedEntityCommandEntry* pCommand;
	unsigned short iEntry;

	pQueue = m_pCommandQueue70;
	if (pQueue != 0 && pQueue->m_cEntries00 < pQueue->m_cCapacity02) {
		iEntry = pQueue->m_cEntries00++;
		pCommand = &pQueue->m_pEntries04[iEntry];
		pCommand->m_nField00 = 1;
		pCommand->m_nField04 = position.x;
		pCommand->m_nField08 = position.y;
		pCommand->m_nField0C = position.z;
	}
}

// Macintosh: CGameObject::GetDestination()
// FUNCTION: LEMBALL 0x00416000
AICOORD* CGameObject::GetDestination(AICOORD* pPosition)
{
	if (m_pCommandQueue70->m_cEntries00 > 0) {
		LevelManagedEntityCommandEntry* pCommand = m_pCommandQueue70->m_pEntries04;
		pPosition->x = pCommand->m_nField04;
		pPosition->y = pCommand->m_nField08;
		pPosition->z = pCommand->m_nField0C;
	}
	else {
		pPosition->x = m_WorldPosition9C.x;
		pPosition->y = m_WorldPosition9C.y;
		pPosition->z = m_WorldPosition9C.z;
	}
	return pPosition;
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
