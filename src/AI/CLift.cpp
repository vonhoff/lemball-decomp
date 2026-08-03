#include "AI/CLift.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_LINKSCF_LiftChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void* g_pLiftTileGrid;

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

// FUNCTION: LEMBALL 0x00424d00
CLift::CLift(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x212, 0, 0);
	*(void**) this = g_LINKSCF_LiftChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x00424d20
CLift::~CLift(void)
{
	*(void**) this = g_LINKSCF_LiftChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x00424d30
void CLift::CalculateCliff(void)
{
	char* pObject;
	char* pGrid;
	char* pTile;
	int nStartX;
	int nStartY;
	int nEndX;
	int x;
	int y;

	pObject = (char*) this;
	nStartX = *(short*) (pObject + 0x13a) / 16;
	nStartY = *(short*) (pObject + 0x13c) / 16;
	nEndX = *(short*) (pObject + 0x140) / 16;
	x = nStartX;
	if (nStartY > 0) {
		for (; x <= nEndX; ++x) {
			pGrid = (char*) g_pLiftTileGrid;
			pTile = *(char**) (pGrid + 0x0c) + ((nStartY - 1) * *(int*) (pGrid + 0x10) + x) * 12;
			*(short*) (pTile + 0x0a) = (*(short*) (pTile + 8) + 15) / 16;
		}
	}
	if (nStartX > 0) {
		for (y = nStartY; y <= nEndX; ++y) {
			pGrid = (char*) g_pLiftTileGrid;
			pTile = *(char**) (pGrid + 0x0c) + (y * *(int*) (pGrid + 0x10) + nStartX) * 12 - 12;
			*(short*) (pTile + 0x0a) = (*(short*) (pTile + 8) + 15) / 16;
		}
	}
}
