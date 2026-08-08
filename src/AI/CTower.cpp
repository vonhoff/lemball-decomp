#include "AI/CTower.h"

extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x0041cf70
int CTower::Process(void)
{
	int xq = m_nPositionX9C >> 12;
	int yq = m_nPositionYA0 >> 12;
	int nTileX = xq >> 4;
	int nTileY = yq >> 4;
	unsigned int z = 0;
	if (!(xq < 0 || yq < 0 ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) (((nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			xq & 0xf, yq & 0xf);
	}
	m_nPositionZA4 = (unsigned int) (z & 0xffff) << 12;
	return 1;
}
