#include "AI/CRocket.h"
#include "AI/CGameObject.h"

extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;

// FUNCTION: LEMBALL 0x00426840
void CRocket::Set(unsigned short nSlotId, const AICOORD& position)
{
	((void(__fastcall*) (void*, unsigned short)) 0x402293)(this, nSlotId);
	m_xPosWorld9C = position.x;
	m_yPosWorldA0 = position.y;
	m_nActive13C = 1;
	m_nStateB8 = 0x18;
	m_zPosWorldA4 = position.z;
	int xtile = (position.x >> 12) / 16;
	if (xtile < 0) return;
	int ytile = (position.y >> 12) / 16;
	if (ytile < 0) return;
	void* pGrid = *(void**) 0x4a74b4;
	if (*(int*) ((char*) pGrid + 0x10) <= xtile) return;
	if (*(int*) ((char*) pGrid + 0x14) <= ytile) return;
	int nIndex = *(int*) ((char*) pGrid + 0x10) * ytile + xtile;
	*(char*) (*(int*) ((char*) pGrid + 0xc) + nIndex * 12 + 7) |= (char) 0x80;
}
// FUNCTION: LEMBALL 0x004269d0
int CRocket::StepOn(const AICOORD& position, CGameObject* pObject)
{
	int nDist = ((int(__cdecl*) (int, int, int, int)) 0x40254a)(m_xPosWorld9C >> 12, m_yPosWorldA0 >> 12, position.x >> 12, position.y >> 12);
	if (nDist < 0x20) {
		m_xPosWorld9C = position.x + 0x4000;
		m_yPosWorldA0 = position.y + 0x4000;
		m_zPosWorldA4 = position.z;
		m_pTarget5C = pObject;
		m_nTargetZ140 = position.z >> 12;
		m_nMotionStartTickC8 = 0x30;
		((void(__fastcall*) (void*, int)) 0x401f3c)(this, 0x1b);
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x00426a60
void CRocket::DoActivate(void)
{
	m_nMotionStartTickC8 += g_nLevelFrameClockTick;
	m_nFrameTime94 = g_nLevelFrameClockTimeMs;
	void* pTarget = m_pTarget5C;
	((void(__fastcall*) (void*, int)) (*(void***) pTarget + 0x8 / 4))(pTarget, 0x15);
	*(int*) ((char*) pTarget + 0xcc) = g_nLevelFrameClockTick + 0x3c;
	((void(__fastcall*) (void*, int)) (*(void***) this + 0x34 / 4))(this, 0x12);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*) (void*, void*)) 0x4032dd)(*(void**) 0x49d128, this);
	}
}
