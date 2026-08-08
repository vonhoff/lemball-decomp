#include "AI/CAmmo.h"

extern int g_nLevelFrameClockTick;
extern void* g_pLevelTileGrid;

struct CGround {
	short GetZThunk(int nLocalX, int nLocalY);
};

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nState);
};

typedef void (__fastcall* CAmmoActionProc)(CAmmo*, int, int);

// FUNCTION: LEMBALL 0x0041cab0
int CAmmo::Process(void)
{
	int nX = m_CurrentPosition9C.m_nX >> 12;
	int nY = m_CurrentPosition9C.m_nY >> 12;
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
	m_CurrentPosition9C.m_nZ = (unsigned int) nHeight << 12;

	int nState = m_nStateB8;
	if (m_fNetworkShadow114 != 0) {
		if (m_nPreviousState128 != nState) {
			if (nState == 0x1a) {
				((CAmmoActionProc) ((void**) *(void**) this)[13])(this, 0, 0x11);
			}
			m_nPreviousState128 = m_nStateB8;
		}
		return 1;
	}

	if (nState == 0x1a) {
		if (g_nLevelFrameClockTick <= (unsigned int) m_nDeadlineD4) {
			return 1;
		}
		if (m_nLinkedVariant144 == 0) {
			*(int*) ((char*) this + 0x38) = 0;
			return 1;
		}
		m_nCycleDeadlineCC = g_nLevelFrameClockTick +
			((unsigned int) (unsigned short) m_nLinkedVariant144 * 1000) / 50;
		nState = 0x1b;
	}
	else {
		if (nState != 0x1b || g_nLevelFrameClockTick <= (unsigned int) m_nCycleDeadlineCC) {
			return 1;
		}
		nState = 0x18;
	}
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(nState);
	return 1;
}