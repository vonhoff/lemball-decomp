#include <stdlib.h>

extern int g_nLevelFrameClockTick;
extern void* g_pLevelTileGrid;
extern void* g_pSessionRandomState;

class CCatapult {
public:
	int Process(void);
};

struct CatapultPoint {
	int x;
	int y;
	int z;
};

static int NextCatapultRandom(void)
{
	int nRandom = (*(int*) g_pSessionRandomState * 0x29 + 0x1f) & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	return nRandom;
}

// FUNCTION: LEMBALL 0x0041c720
int CCatapult::Process(void)
{
	int* pThis = (int*) this;
	int nPixelX = pThis[0x27] >> 12;
	int nPixelY = pThis[0x28] >> 12;
	int nTileX = nPixelX >> 4;
	int nTileY = nPixelY >> 4;
	unsigned short nHeight = 0;
	char* pGrid = (char*) g_pLevelTileGrid;
	if (nPixelX >= 0 && nPixelY >= 0 &&
		nTileX < *(int*) (pGrid + 0x10) && nTileY < *(int*) (pGrid + 0x14)) {
		void* pTile = (void*) (*(int*) (pGrid + 0x0c) +
			(nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0x0c);
		nHeight = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
			pTile, 0, nPixelX & 0x0f, nPixelY & 0x0f);
	}
	int nGroundZ = (unsigned int) nHeight << 12;
	pThis[0x29] = nGroundZ;

	if (pThis[0x45] != 0) {
		*(short*) (pThis + 0x2f) = 1;
		if (pThis[0x4a] != pThis[0x2e]) {
			if (pThis[0x2e] == 0x1b) {
				((void(__fastcall*)(void*, int, int)) (*(void***) this)[0x34 / 4])(this, 0, 9);
			}
			pThis[0x4a] = pThis[0x2e];
		}
		return 1;
	}

	*(short*) (pThis + 0x2f) = 0;
	switch (pThis[0x2e]) {
	case 0x19:
		if ((unsigned int) pThis[0x34] < (unsigned int) g_nLevelFrameClockTick) {
			((void(__fastcall*)(void*, int, int)) (*(void***) this)[2])(this, 0, 0x1a);
		}
		break;
	case 0x1a:
		if ((unsigned int) pThis[0x35] < (unsigned int) g_nLevelFrameClockTick) {
			CatapultPoint Origin;
			CatapultPoint Velocity;
			Origin.x = pThis[0x27] - 0xc000;
			Origin.y = pThis[0x28] - 0xc000;
			Origin.z = nGroundZ + 0x20000;

			int nRandomX = abs(NextCatapultRandom()) & 0x7fff;
			int nRandomY = abs(NextCatapultRandom()) & 0x7fff;
			int nRandomZ = abs(NextCatapultRandom()) & 0x7fff;
			int nFixedX = nRandomX << 12;
			int nFixedY = nRandomY << 12;
			int nFixedZ = nRandomZ << 12;
			Velocity.x = ((nFixedX + (nFixedX >> 31 & 0x7fff)) >> 15) + 0x9000;
			Velocity.y = (nFixedY + (nFixedY >> 31 & 0x7fff)) >> 15;
			Velocity.z = ((nFixedZ + (nFixedZ >> 31 & 0x7fff)) >> 15) + 0xc000;

			void* pTarget = (void*) pThis[0x17];
			*(int*) ((char*) pTarget + 0xc0) = 0;
			*(int*) ((char*) pTarget + 0xb8) = 0;
			((void(__fastcall*)(void*, int, CatapultPoint*, CatapultPoint*)) (*(void***) pTarget)[0x20 / 4])(
				pTarget, 0, &Velocity, &Origin);
			pThis[0x17] = 0;
			void** pVtable = *(void***) this;
			((void(__fastcall*)(void*, int, int)) pVtable[2])(this, 0, 0x1b);
			((void(__fastcall*)(void*, int, int)) pVtable[0x34 / 4])(this, 0, 9);
		}
		break;
	case 0x1b:
		if ((unsigned int) pThis[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			((void(__fastcall*)(void*, int, int)) (*(void***) this)[2])(this, 0, 0x18);
		}
		break;
	}
	return 1;
}
