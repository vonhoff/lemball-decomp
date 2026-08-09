#include "AI/CSlinky.h"

extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x0040b4d0
void CSlinky::Set(int nSlinkX, int nSlinkY, int nSlinkZ, int nSlinkW)
{
	m_setTargetX124 = nSlinkX;
	m_setTargetY128 = nSlinkZ;
	m_setTarget2C12C = nSlinkY;
	m_setTarget2D130 = nSlinkW;
	unsigned int z = 0;
	if (nSlinkX >= 0 && nSlinkZ >= 0 &&
	    (nSlinkX >> 4) < *(int*) ((char*) g_pLevelTileGrid + 0x10) &&
	    (nSlinkZ >> 4) < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) ((((nSlinkZ >> 4) * *(int*) ((char*) g_pLevelTileGrid + 0x10) + (nSlinkX >> 4)) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			nSlinkX & 0xf, nSlinkZ & 0xf);
	}
	m_zPosWorldA4 = (z & 0xffff) << 12;
	m_xPosWorld9C = m_setTargetX124 << 12;
	m_yPosWorldA0 = m_setTargetY128 << 12;
	m_nFrameClockTickCC = g_nLevelFrameClockTick;
	m_motionTimeMs94 = g_nLevelFrameClockTimeMs;
	((void(__fastcall*)(void*, int)) (*(void***) this)[2])(this, 0x18);
}

// FUNCTION: LEMBALL 0x0040b670
void CSlinky::Move(void)
{
	int* pState = *(int**) 0x4a1bcc;
	int v = (*pState * 45 + 31) & 0x7fffff;
	*pState = v;
	int heading = v % 4;
	m_wFacingDirBC = (short) heading;
	int dx, dy;
	switch (m_wFacingDirBC) {
	case 0:
		dx = 0x10; dy = 0;
		break;
	case 1:
		dx = -0x10; dy = 0;
		break;
	case 2:
		dx = 0; dy = 0x10;
		break;
	case 3:
		dx = 0; dy = -0x10;
		break;
	}
	int i = 0;
	do {
		i++;
		m_moveTargetXA8 = ((m_xPosWorld9C >> 12) + dx) << 12;
		m_moveTargetYAC = ((m_yPosWorldA0 >> 12) + dy) << 12;
		m_moveTargetZB0 = (m_zPosWorldA4 >> 12) << 12;
	} while (i < 8 && ((int(__fastcall*)(void*, int*)) 0x403229)(this, (int*) &m_moveTargetXA8) == 0);
}

// FUNCTION: LEMBALL 0x0040b760
int CSlinky::Process(void)
{
	typedef void(__fastcall * NoArgProc)(void*);
	typedef void(__fastcall * SetStateProc)(void*, int, int);
	typedef int(__fastcall * IntersectsProc)(void*, int, int*);
	int* pOwner;
	int* pObject;
	int* pHit;
	int* pBounds;
	int i;

	if (m_fieldB8 == 0x18) {
		if ((unsigned int) g_nLevelFrameClockTick >= (unsigned int) m_nFrameClockTickCC) {
			((NoArgProc) ((void**) m_vtable)[0x44 / 4])(this);
			m_motionTimeMs94 = g_nLevelFrameClockTimeMs;
			((SetStateProc) ((void**) m_vtable)[2])(this, 0, 0x1b);
			m_nFrameClockTickCC = g_nLevelFrameClockTick + 0x10;
		}
	}
	else if (m_fieldB8 == 0x1b && (unsigned int) g_nLevelFrameClockTick >= (unsigned int) m_nFrameClockTickCC) {
		m_xPosWorld9C = m_moveTargetXA8;
		m_yPosWorldA0 = m_moveTargetYAC;
		m_zPosWorldA4 = m_moveTargetZB0;
		m_motionTimeMs94 = g_nLevelFrameClockTimeMs;
		((SetStateProc) ((void**) m_vtable)[2])(this, 0, 0x18);
		m_nFrameClockTickCC = g_nLevelFrameClockTick + 0x14;
	}

	pOwner = *(int**) 0x004A74B0;
	pBounds = pOwner + 0x134 / 4;
	pBounds[0] = (m_xPosWorld9C >> 12) - 4;
	pBounds[1] = (m_yPosWorldA0 >> 12) - 4;
	pBounds[2] = (m_zPosWorldA4 >> 12) - 4;
	pBounds[3] = (m_xPosWorld9C >> 12) + 3;
	pBounds[4] = (m_yPosWorldA0 >> 12) + 3;
	pBounds[5] = (m_zPosWorldA4 >> 12) + 3;
	pOwner[0x150 / 4] = (int) this;
	pOwner[0x14c / 4] = 0;
	pHit = 0;
	i = 0;
	while (i < pOwner[0x118 / 4]) {
		pObject = *(int**) (pOwner[0x120 / 4] + i * 4);
		if (pObject != (int*) pOwner[0x150 / 4] &&
			((IntersectsProc) ((void**) *pObject)[0x4c / 4])(pObject, 0, pBounds) != 0) {
			++pOwner[0x14c / 4];
			pHit = pObject;
			break;
		}
		++i;
		pOwner[0x14c / 4] = i;
	}
	if (pHit != 0) {
		((NoArgProc) ((void**) *pHit)[0x58 / 4])(pHit);
	}
	return 1;
}
