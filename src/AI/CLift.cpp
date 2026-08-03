#include "AI/CLift.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_LINKSCF_LiftChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void* g_pLiftTileGrid;
extern int LEMBALL_FASTCALL RequestLiftChunkObjectActivationState(void* pObject);

// GLOBAL: LEMBALL 0x0049e1c0
unsigned short g_nNextLiftObjectId = 0;

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

// FUNCTION: LEMBALL 0x00424df0
void CLift::Edit(int nHeight,
				 short nDirection,
				 int nTargetA,
				 int nTargetB,
				 eLiftActivateType activateType,
				 unsigned char nFlags)
{
	char* pObject;
	char* pGrid;
	char* pTile;
	int x;
	int y;
	int nTileX;
	int nTileY;
	int i;

	pObject = (char*) this;
	if (nTargetA == -1) {
		nTargetA = nHeight;
	}
	*(short*) (pObject + 0x154) = nDirection;
	*(int*) (pObject + 0x148) = nTargetA;
	*(int*) (pObject + 0x168) = nFlags;
	*(int*) (pObject + 0x14c) = nTargetB;
	*(int*) (pObject + 0x15c) = activateType;
	*(int*) (pObject + 0xb8) = 0x18;
	*(int*) (pObject + 0x16c) = 0;
	for (i = 0; i < 8; ++i) {
		*(void**) (pObject + 0x170 + i * 4) = 0;
	}
	*(short*) (pObject + 0x13e) = (short) nHeight;
	*(short*) (pObject + 0x144) = (short) nHeight;
	if (*(int*) (pObject + 0x148) == (short) nHeight && *(short*) (pObject + 0x154) != 1) {
		*(short*) (pObject + 0x154) = 1;
	}
	else if (*(int*) (pObject + 0x14c) == (short) nHeight && *(short*) (pObject + 0x154) == 1) {
		*(short*) (pObject + 0x154) = -1;
	}
	for (x = *(short*) (pObject + 0x13a); x <= *(short*) (pObject + 0x140); x += 16) {
		nTileX = x / 16;
		for (y = *(short*) (pObject + 0x13c); y <= *(short*) (pObject + 0x142); y += 16) {
			nTileY = y / 16;
			switch (activateType) {
			case 0:
			case 1:
			case 3:
				*(int*) (pObject + 0x164) = 0;
				break;
			case 2:
				RequestLiftChunkObjectActivationState(this);
				break;
			case 4:
				*(int*) (pObject + 0x164) = 0;
				*(int*) (pObject + 0x168) = 0;
				break;
			}
			pGrid = (char*) g_pLiftTileGrid;
			if (nTileX >= 0 && nTileY >= 0 && nTileX < *(int*) (pGrid + 0x10) && nTileY < *(int*) (pGrid + 0x14)) {
				pTile = *(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12;
				*(unsigned short*) (pTile + 6) |= 0x8020;
			}
			pTile = *(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12;
			*(char**) (pObject + 0x160) = pTile;
			*(short*) (pTile + 8) = (short) nHeight;
			*(short*) (pTile + 0x0a) = (nHeight + 15) / 16;
		}
	}
	CalculateCliff();
}

// FUNCTION: LEMBALL 0x00425010
void CLift::Set(int x,
				int y,
				int z,
				short nDirection,
				int nTargetA,
				int nTargetB,
				eLiftActivateType activateType,
				unsigned char nFlags)
{
	tCoord3d position;
	position.x = (short) x;
	position.y = (short) y;
	position.z = (short) z;
	Set(position, position, nDirection, nTargetA, nTargetB, activateType, nFlags);
}

// FUNCTION: LEMBALL 0x00425060
void CLift::Set(tCoord3d& start,
				tCoord3d& end,
				short nDirection,
				int nTargetA,
				int nTargetB,
				eLiftActivateType activateType,
				unsigned char nFlags)
{
	char* pObject;
	pObject = (char*) this;
	*(int*) (pObject + 0x9c) = (int) start.x << 12;
	*(int*) (pObject + 0xa0) = (int) start.y << 12;
	*(int*) (pObject + 0xa4) = (int) start.z << 12;
	*(unsigned short*) (pObject + 0x138) = g_nNextLiftObjectId++;
	*(tCoord3d*) (pObject + 0x13a) = start;
	*(tCoord3d*) (pObject + 0x140) = end;
	Edit(*(short*) (pObject + 0x13e), nDirection, nTargetA, nTargetB, activateType, nFlags);
}
