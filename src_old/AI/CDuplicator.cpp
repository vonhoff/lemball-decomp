#include "AI/CDuplicator.h"

#include "AI/CGameObject.h"

extern void* g_pLevelTileGrid;
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pActiveManagedEntityOwner;

// FUNCTION: LEMBALL 0x004275f0
void CDuplicator::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	m_nActionPhaseBC = 0;
	m_nFrameTime94 = 0;
	m_nActive13C = 0;
	m_nPlaced138 = 0;
	m_nStateB8 = 0x18;
	((void(__fastcall*)(void*, void*)) 0x4023e2)(this, m_anInitialPosition40);
}
// FUNCTION: LEMBALL 0x00427630
void CDuplicator::Set(const AICOORD& position)
{
	int nWorldX = position.x;
	m_xPosWorld9C = nWorldX;
	int nWorldY = position.y;
	m_yPosWorldA0 = nWorldY;
	int nWorldZ = position.z;
	m_nPlaced138 = 1;
	m_nActive13C = 1;
	m_zPosWorldA4 = nWorldZ;
	nWorldX = (nWorldX >> 12) / 16;
	nWorldY = (nWorldY >> 12) / 16;
	if (nWorldX > -1) {
		if (nWorldY > -1 && nWorldX < *(int*) ((char*) g_pLevelTileGrid + 0x10) && nWorldY < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nWorldY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nWorldX) * 0xc) |= 1;
		}
		nWorldY--;
		if (nWorldY > -1) {
			if (nWorldX < *(int*) ((char*) g_pLevelTileGrid + 0x10) && nWorldY < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * nWorldY + nWorldX) * 0xc) |= 1;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004276f0
void CDuplicator::Delete(void)
{
	int nTileX = (m_xPosWorld9C >> 12) / 16;
	int nTileY = (m_yPosWorldA0 >> 12) / 16;
	if (nTileX > -1) {
		if (nTileY > -1 && nTileX < *(int*) ((char*) g_pLevelTileGrid + 0x10) && nTileY < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * nTileY + nTileX) * 0xc) &= 0xfffe;
		}
		nTileY--;
		if (nTileY > -1) {
			if (nTileX < *(int*) ((char*) g_pLevelTileGrid + 0x10) && nTileY < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * nTileY + nTileX) * 0xc) &= 0xfffe;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00427910
void CDuplicator::DoActivate(void)
{
	CGameObject* pChild;
	int nYOffset;

	nYOffset = m_yPosWorldA0 - 0x3c000;
	m_nFrameTime94 = g_nLevelFrameClockTimeMs;
	pChild = m_pChild5C;
	m_nNextUpdateTickCC += g_nLevelFrameClockTick;
	m_pActivatedChild140 = pChild;
	pChild->m_nRuntimeFieldC0 = 1;
	pChild->m_nStateB8 = 5;
	pChild = m_pActivatedChild140;
	pChild->m_WorldPosition9C.x = m_xPosWorld9C;
	pChild->m_WorldPosition9C.y = nYOffset;
	pChild->m_WorldPosition9C.z = m_zPosWorldA4;
	((void(__fastcall*)(void*))((void**) m_vtable00)[0x34 / 4])(this);
	((void(__fastcall*)(void*, int, int)) 0x402f22)(g_pActiveManagedEntityOwner, 0, 100);
}
