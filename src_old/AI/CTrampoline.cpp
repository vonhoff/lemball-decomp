#include "AI/CTrampoline.h"
#include "AI/CGameObject.h"
#include "Visos/Generic/Memory.h"

extern void* g_pLevelTileGrid;
extern void* g_pActiveManagedEntityOwner;

// Mac blueprint: CTrampoline::Set(ushort, CGRect-like fixed-point point)

// FUNCTION: LEMBALL 0x0042a9e0
void CTrampoline::Set(unsigned short nSlot, const int* pPoint)
{
	char* pObjectBytes = (char*) this;
	unsigned char* pCellFlags;
	int nTileX;
	int nTileY;
	int nCellColumns;
	int nCellRows;

	((CGameObject*) this)->SetId(nSlot);
	*(int*) (pObjectBytes + 0x9c) = pPoint[0];
	*(int*) (pObjectBytes + 0xa0) = pPoint[1];
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	*(int*) (pObjectBytes + 0xa4) = pPoint[2];
	*(int*) (pObjectBytes + 0x13c) = 1;
	*(int*) (pObjectBytes + 0x138) = 1;
	nTileX = (pPoint[0] >> 12) + ((pPoint[0] >> 12) >> 31 & 0xf);
	nTileX = nTileX >> 4;
	nTileY = (pPoint[1] >> 12) + ((pPoint[1] >> 12) >> 31 & 0xf);
	nTileY = nTileY >> 4;
	if (nTileX >= 0 && nTileY >= 0) {
		nCellColumns = *(int*) ((char*) g_pLevelTileGrid + 0x10);
		nCellRows = *(int*) ((char*) g_pLevelTileGrid + 0x14);
		if (nTileX < nCellColumns && nTileY < nCellRows) {
			pCellFlags = (unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 7 + (nCellColumns * nTileY + nTileX) * 0xc);
			*pCellFlags = (unsigned char) (*pCellFlags | 0x80);
		}
	}
}
