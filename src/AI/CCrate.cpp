#include "AI/CCrate.h"

extern int g_nLevelFrameClockTick;
extern void* g_pLevelTileGrid;

struct CGround {
	short GetZThunk(int nLocalX, int nLocalY);
};

typedef void (__fastcall* CCrateActionProc)(CCrate*, int, int);

// FUNCTION: LEMBALL 0x0041cd20
int CCrate::Process(void)
{
	int nX = m_nPositionX9C >> 12;
	int nY = m_nPositionYA0 >> 12;
	int nTileX = nX >> 4;
	int nTileY = nY >> 4;
	unsigned short nHeight = 0;
	if (nX >= 0 && nY >= 0 &&
		nTileX < *(int*) ((char*) g_pLevelTileGrid + 0x10) &&
		nTileY < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
		CGround* pGround = (CGround*)
			(*(int*) ((char*) g_pLevelTileGrid + 0xc) +
			 (nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc);
		nHeight = pGround->GetZThunk(nX & 0xf, nY & 0xf);
	}
	m_nPositionZA4 = (unsigned int) nHeight << 12;

	int nState = m_nStateB8;
	if (m_fNetworkShadow114 != 0) {
		if (m_nState128 != nState) {
			if (nState == 0x19) {
				((CCrateActionProc) ((void**) *(void**) this)[13])(this, 0, 0x14);
			}
			else if (nState == 0x1a) {
				TriggerContents();
				((CCrateActionProc) ((void**) *(void**) this)[13])(this, 0, 10);
			}
			m_nState128 = m_nStateB8;
		}
		return 1;
	}

	if (nState == 0x19) {
		if ((unsigned int) m_nPhaseStepD0 < g_nLevelFrameClockTick) {
			TriggerContents();
			((CCrateActionProc) ((void**) *(void**) this)[13])(this, 0, 10);
			((CCrateActionProc) ((void**) *(void**) this)[2])(this, 0, 0x1a);
		}
	}
	else if (nState == 0x1a && (unsigned int) m_nDeadlineD4 < g_nLevelFrameClockTick) {
		((CCrateActionProc) ((void**) *(void**) this)[2])(this, 0, 0x18);
		*(int*) ((char*) this + 0x38) = 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0041ccc0
void CCrate::TriggerContents(void)
{
	if (m_nContentsType14C != 0xffff) {
		void* pvVar1 = m_pContents148;
		m_nPositionX9C = *(int*) ((char*) pvVar1 + 0x9c);
		m_nPositionYA0 = *(int*) ((char*) pvVar1 + 0xa0);
		m_nPositionZA4 = *(int*) ((char*) pvVar1 + 0xa4);
		((void(__fastcall*)(void*, int, void*, int)) 0x402cac)(*(void**) 0x4a74c0, -1, pvVar1, 0);
		m_nContentsType14C = 0xffff;
	}
}
