#include "AI/CTrapDoor.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "views/2d/C2DRender.h"

extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern int g_nNetworkFrameClockTimeMs;

struct CGround {
	char m_abData00[0x0c];
	short GetZThunk(int nLocalX, int nLocalY);
};

struct TrapDoorTileGridView {
	char m_abReserved00[0x0c];
	CGround* m_pGround0C;
	int m_nWidth10;
	int m_nHeight14;
};

// FUNCTION: LEMBALL 0x0040c3b0
void CTrapDoor::GetViewData(CViewData& ViewData)
{
	typedef void(LEMBALL_FASTCALL* SetObjectFieldProc)(CTrapDoor*, int, int);
	TrapDoorTileGridView* pGrid = (TrapDoorTileGridView*) g_pLevelTileGrid;
	char* pEntry = (char*) &ViewData;
	int nY = m_WorldPosition9C.y >> 12;
	int nX = m_WorldPosition9C.x >> 12;
	int nTileX = nX >> 4;
	int nTileY = nY >> 4;
	unsigned short nHeight = 0;

	if (nX >= 0 && nY >= 0 && nTileX < pGrid->m_nWidth10 && nTileY < pGrid->m_nHeight14) {
		nHeight = pGrid->m_pGround0C[nTileY * pGrid->m_nWidth10 + nTileX]
						  .GetZThunk(nX & 0x0f, nY & 0x0f);
	}
	m_WorldPosition9C.z = ((unsigned int) nHeight + 0x4e) << 12;

	*(unsigned short*) (pEntry + 0x2c) = m_nRegistryIndex6A;
	*(unsigned short*) (pEntry + 0x2e) = 0;
	*(int*) (pEntry + 0x28) = m_nEntityType64;
	*(int*) (pEntry + 0x04) = m_WorldPosition9C.x >> 12;
	*(int*) (pEntry + 0x08) = m_WorldPosition9C.y >> 12;
	*(int*) (pEntry + 0x0c) = m_WorldPosition9C.z >> 12;
	*(unsigned short*) pEntry = (unsigned short) m_nHeadingOctantB4;
	*(unsigned short*) (pEntry + 0x1c) = (unsigned short) m_nActionPhaseBC;
	*(int*) (pEntry + 0x18) = m_nStateB8;
	*(int*) (pEntry + 0x20) = m_nFrameTime94;
	*(int*) (pEntry + 0x14) = 0;
	*(int*) (pEntry + 0x30) = m_nRuntimeFieldC0;
	*(int*) (pEntry + 0x38) = m_nJumpFieldE4;
	*(int*) (pEntry + 0x3c) = m_nJumpFieldE8;
	*(int*) (pEntry + 0x40) = m_nJumpFieldEC;
	*(int*) (pEntry + 0x44) = m_nRuntimeField98;
	*(int*) (pEntry + 0x24) =
		m_nPendingState114 != 0 ? g_nNetworkFrameClockTimeMs : g_nLevelFrameClockTimeMs;
	((SetObjectFieldProc) ((void**) m_pVtable00)[13])(this, m_nRuntimeField98, 0);
	*(int*) (pEntry + 0x34) = m_nRuntimeField10;
	m_nRuntimeField10 = 0;
}

// FUNCTION: LEMBALL 0x0040c4f0
int CTrapDoor::Process(void)
{
	typedef void(LEMBALL_FASTCALL * SetStateProc)(CTrapDoor*, int, int);
	SetStateProc pSetState = (SetStateProc) ((void**) m_pVtable00)[2];
	SetStateProc pSetEffect = (SetStateProc) ((void**) m_pVtable00)[13];
	int& fLocalEffect = *(int*) 0x0049CF3C;
	int& fNetworkEffect = *(int*) 0x0049CF40;
	int fFinished;

	if (m_fTransitionActive148 == 0) {
		return 1;
	}

	if (m_nPendingState114 != 0) {
		fFinished = 0;
		if (m_nPreviousNetworkState128 == m_nStateB8) {
			return 1;
		}

		if (m_nStateB8 == 0x1e) {
			fFinished = 1;
		}
		else if (m_nStateB8 == 0x20) {
			if (fNetworkEffect == 0) {
				fNetworkEffect = 1;
				pSetEffect(this, 0, 0x18);
			}
		}
		else if (m_nStateB8 == 0x22 && fNetworkEffect == 1) {
			fNetworkEffect = 0;
			pSetEffect(this, 0, 0x18);
		}

		m_nPreviousNetworkState128 = m_nStateB8;
		return !fFinished;
	}

	if (m_nVariant144 != 0) {
		return 1;
	}
	if ((unsigned int) m_nNextUpdateTickCC > (unsigned int) g_nLevelFrameClockTick) {
		return 1;
	}

	m_nFrameTime94 = g_nLevelFrameClockTimeMs;
	switch (m_nStateB8) {
	case 0x18:
		if (fLocalEffect == 0) {
			pSetEffect(this, 0, 0x20);
			fLocalEffect = 1;
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x36;
		pSetState(this, 0, 0x1f);
		break;
	case 0x1f:
		if (fLocalEffect == 1) {
			fLocalEffect = 0;
			pSetEffect(this, 0, 0x18);
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x14;
		pSetState(this, 0, 0x20);
		break;
	case 0x20:
		if (fLocalEffect == 0) {
			fLocalEffect = 1;
			pSetEffect(this, 0, 1);
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x50;
		pSetState(this, 0, 0x21);
		break;
	case 0x21:
		if (fLocalEffect == 1) {
			fLocalEffect = 0;
			pSetEffect(this, 0, 0x18);
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x14;
		pSetState(this, 0, 0x22);
		break;
	case 0x22:
		if (fLocalEffect == 0) {
			pSetEffect(this, 0, 0x21);
		}
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x36;
		pSetState(this, 0, 0x23);
		break;
	case 0x23:
		pSetState(this, 0, 0x1e);
		return 0;
	}

	return 1;
}
