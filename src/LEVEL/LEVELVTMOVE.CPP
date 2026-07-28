#include "ENGINE/CORE/VSINIT.H"

extern int g_nLevelFrameClockTick;

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
	unsigned short m_nReserved02;
	LevelManagedEntityCommandEntry* m_pEntries04;
};

struct LevelManagedEntityMoveCommandView {
	char m_abReserved00[0x70];
	LevelManagedEntityCommandQueue* m_pCommandQueue70;
	int m_fHasCommands74;
	char m_abReserved78[0x10];
	int m_nMotionDuration88;
	char m_abReserved8C[0x10];
	LevelThreeDwordPoint m_WorldPosition9C;
	char m_abReservedA8[0x20];
	int m_nMotionStartTickC8;
	int m_nNextUpdateTickCC;

	void PopFirstMoveCommand(void);
	void PopFirstMoveCommandAndResetMotion(void);
	LevelThreeDwordPoint* CopyFirstMoveCommandPositionOrCurrent(LevelThreeDwordPoint* pPosition);
};

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation in LEVELVT.CPP.

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
