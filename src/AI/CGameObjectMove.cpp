#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern int g_nLevelFrameClockTick;
extern unsigned int __cdecl compute_direction_octant_between_points(int nX1, int nY1, int nX2, int nY2);

// GLOBAL: LEMBALL 0x0049d020
static const int g_LEVEL_HeadingTurnDirections[8] = {0, 1, 1, 1, 1, -1, -1, -1};

// GLOBAL: LEMBALL 0x0049d0b0
static const int g_LEVEL_ManagedEntityUpdatePeriods[9] = {0, 87, 75, 0, 0, 0, 0, 75, 75};

struct LevelThreeDwordPoint {
	int m_nX;
	int m_nY;
	int m_nZ;
};

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

struct LevelManagedEntityMoveCommandView {
	char m_abReserved00[0x64];
	int m_nEntityType64;
	char m_abReserved68[8];
	LevelManagedEntityCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	char m_abReserved78[0x10];
	int m_nMotionDuration88;
	char m_abReserved8C[0x10];
	LevelThreeDwordPoint m_WorldPosition9C;
	char m_abReservedA8[0xc];
	short m_nHeadingOctantB4;
	char m_abReservedB6[0x12];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;

	void PopFirstMoveCommand(void);
	void PopFirstMoveCommandAndResetMotion(void);
	void UpdateFacingTowardFirstMoveCommand(void);
	int IsFacingTowardFirstMoveCommand(void);
	void AppendMoveCommand(const LevelThreeDwordPoint* pPosition);
	LevelThreeDwordPoint* CopyFirstMoveCommandPositionOrCurrent(LevelThreeDwordPoint* pPosition);
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
void LevelManagedEntityMoveCommandView::PopFirstMoveCommandAndResetMotion(void)
{
	int nFrameClockTick;
	PopFirstMoveCommand();
	m_nMotionDuration88 = 0;
	nFrameClockTick = g_nLevelFrameClockTick;
	m_nNextUpdateTickCC = nFrameClockTick;
	m_nMotionStartTickC8 = nFrameClockTick;
}

// Macintosh: CGameObject::TurnToFaceDestination()
// FUNCTION: LEMBALL 0x00415d90
void LevelManagedEntityMoveCommandView::UpdateFacingTowardFirstMoveCommand(void)
{
	LevelThreeDwordPoint targetPosition;
	int nDirection;

	CopyFirstMoveCommandPositionOrCurrent(&targetPosition);
	nDirection = compute_direction_octant_between_points(m_WorldPosition9C.m_nX >> 12,
														 m_WorldPosition9C.m_nY >> 12,
														 targetPosition.m_nX >> 12,
														 targetPosition.m_nY >> 12);
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
int LevelManagedEntityMoveCommandView::IsFacingTowardFirstMoveCommand(void)
{
	LevelThreeDwordPoint targetPosition;
	int nDirection;

	CopyFirstMoveCommandPositionOrCurrent(&targetPosition);
	nDirection = compute_direction_octant_between_points(m_WorldPosition9C.m_nX >> 12,
														 m_WorldPosition9C.m_nY >> 12,
														 targetPosition.m_nX >> 12,
														 targetPosition.m_nY >> 12);
	return m_nHeadingOctantB4 == nDirection;
}

// Macintosh: CGameObject::DeleteFirstEntryFromDestinationList()
// FUNCTION: LEMBALL 0x00415e80
void LevelManagedEntityMoveCommandView::PopFirstMoveCommand(void)
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
void LevelManagedEntityMoveCommandView::AppendMoveCommand(const LevelThreeDwordPoint* pPosition)
{
	LevelManagedEntityCommandQueue* pQueue;
	LevelManagedEntityCommandEntry* pCommand;
	unsigned short iEntry;

	pQueue = m_pCommandQueue70;
	if (pQueue != 0 && pQueue->m_cEntries00 < pQueue->m_cCapacity02) {
		iEntry = pQueue->m_cEntries00++;
		pCommand = &pQueue->m_pEntries04[iEntry];
		pCommand->m_nField00 = 1;
		pCommand->m_nField04 = pPosition->m_nX;
		pCommand->m_nField08 = pPosition->m_nY;
		pCommand->m_nField0C = pPosition->m_nZ;
	}
}

// Macintosh: CGameObject::GetDestination()
// FUNCTION: LEMBALL 0x00416000
LevelThreeDwordPoint* LevelManagedEntityMoveCommandView::CopyFirstMoveCommandPositionOrCurrent(
	LevelThreeDwordPoint* pPosition)
{
	if (m_pCommandQueue70->m_cEntries00 > 0) {
		LevelManagedEntityCommandEntry* pCommand = m_pCommandQueue70->m_pEntries04;
		pPosition->m_nX = pCommand->m_nField04;
		pPosition->m_nY = pCommand->m_nField08;
		pPosition->m_nZ = pCommand->m_nField0C;
	}
	else {
		pPosition->m_nX = m_WorldPosition9C.m_nX;
		pPosition->m_nY = m_WorldPosition9C.m_nY;
		pPosition->m_nZ = m_WorldPosition9C.m_nZ;
	}
	return pPosition;
}

// FUNCTION: LEMBALL 0x00419ea0
void __cdecl ThunkPopFirstManagedEntityMoveCommandAndResetMotion(void* pUnused,
																 LevelManagedEntityMoveCommandView* pEntity)
{
	(void) pUnused;
	pEntity->PopFirstMoveCommandAndResetMotion();
}

// FUNCTION: LEMBALL 0x00419ec0
void __cdecl ThunkUpdateManagedEntityFacingTowardFirstMoveCommand(void* pUnused,
																  LevelManagedEntityMoveCommandView* pEntity)
{
	(void) pUnused;
	pEntity->UpdateFacingTowardFirstMoveCommand();
}
