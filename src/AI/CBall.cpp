#include "AI/CBall.h"

extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x004216c0
void CBall::Set(AICOORD oCurrent, AICOORD oTarget, int nSpeed)
{
	m_nPositionX9C = oCurrent.x;
	m_nPositionYA0 = oCurrent.y;
	m_nPositionZA4 = oCurrent.z;
	m_nSavedPosX40 = oCurrent.x;
	m_nSavedPosY44 = oCurrent.y;
	m_nSavedPosZ48 = oCurrent.z;
	m_nTargetX124 = oTarget.x;
	m_nTargetY128 = oTarget.y;
	m_nTargetZ12C = oTarget.z;
	m_nStateB8 = 0x25;
	m_wDirectionBC = 0;
	if (*(unsigned short*) ((char*) g_pActiveManagedEntityOwner + 0x54) < 7) {
		m_wSpeed130 = (short) *(int*) (m_nObjectType64 * 4 + 0x49d070);
	}
	else {
		m_wSpeed130 = (short) nSpeed;
	}
	if (*(unsigned short*) ((char*) this + 0x130) > 1) {
		m_nActive134 = 1;
	}
	else {
		m_wSpeed130 = (short) *(int*) (m_nObjectType64 * 4 + 0x49d070);
		m_nActive134 = 1;
	}
	m_nActive134 = 1;
}

// FUNCTION: LEMBALL 0x00421b40
void CBall::SetHeightCorrect(void)
{
	int xq = m_nPositionX9C >> 12;
	int yq = m_nPositionYA0 >> 12;
	int nTileX = m_nPositionX9C >> 16;
	int nTileY = m_nPositionYA0 >> 16;
	unsigned int z = 0;
	if (!(xq < 0 || yq < 0 || *(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
		  *(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) (((nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc) +
					 *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			xq & 0xf,
			yq & 0xf);
	}
	*(unsigned int*) ((char*) this + 0xa4) = (z & 0xffff) << 12;
}
