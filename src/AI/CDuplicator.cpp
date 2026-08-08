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
	int iVar3 = position.x;
	m_xPosWorld9C = iVar3;
	int iVar4 = position.y;
	m_yPosWorldA0 = iVar4;
	int iVar2 = position.z;
	m_nPlaced138 = 1;
	m_nActive13C = 1;
	m_zPosWorldA4 = iVar2;
	iVar3 = (iVar3 >> 12) / 16;
	iVar4 = (iVar4 >> 12) / 16;
	if (iVar3 > -1) {
		if (iVar4 > -1 && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar4 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (iVar4 * *(int*) ((char*) g_pLevelTileGrid + 0x10) + iVar3) * 0xc) |= 1;
		}
		iVar4--;
		if (iVar4 > -1) {
			if (iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar4 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar4 + iVar3) * 0xc) |= 1;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004276f0
void CDuplicator::Delete(void)
{
	int iVar2 = (m_xPosWorld9C >> 12) / 16;
	int iVar3 = (m_yPosWorldA0 >> 12) / 16;
	if (iVar2 > -1) {
		if (iVar3 > -1 && iVar2 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar3 + iVar2) * 0xc) &= 0xfffe;
		}
		iVar3--;
		if (iVar3 > -1) {
			if (iVar2 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar3 + iVar2) * 0xc) &= 0xfffe;
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
