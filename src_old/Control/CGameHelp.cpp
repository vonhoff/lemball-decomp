#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

// Split from the original GAME source group to preserve MSVC 4.20 code generation in GAME.CPP.

// GLOBAL: LEMBALL 0x0049d040
static const unsigned int g_GAME_PositiveYDirectionMap[8] = {4, 3, 2, 3, 4, 5, 6, 5};

// MACINTOSH: ReturnFacingDirection(int, int, int, int)
// FUNCTION: LEMBALL 0x00413e80
unsigned int __cdecl ReturnFacingDirection(int nX1, int nY1, int nX2, int nY2)
{
	int nDeltaX;
	int nDeltaY;
	int nPositiveTemporary;
	int nNegativeTemporary;
	int* pMagnitude;
	int nAbsY;
	int nAbsX;
	int nFraction;
	unsigned int nDirection;

	nDeltaX = (nX2 - nX1) << 12;
	nDeltaY = (nY2 - nY1) << 12;

	if (nDeltaX < 0) {
		nNegativeTemporary = -nDeltaX;
		pMagnitude = &nNegativeTemporary;
	}
	else {
		pMagnitude = &nPositiveTemporary;
		nPositiveTemporary = nDeltaX;
	}
	nAbsX = *pMagnitude;

	if (nDeltaY < 0) {
		nNegativeTemporary = -nDeltaY;
		pMagnitude = &nNegativeTemporary;
	}
	else {
		pMagnitude = &nPositiveTemporary;
		nPositiveTemporary = nDeltaY;
	}
	nAbsY = *pMagnitude;

	nFraction = ((nAbsY & 0xfff) * 0x6a0) >> 12;

	if (nAbsX < (nAbsY >> 12) * 0x6a0 + nFraction) {
		nDirection = 0;
	}
	else {
		nDirection = 1;
		if (((nAbsY >> 12) * 0x350 + nAbsY) * 2 + nFraction <= nAbsX) {
			nDirection = 2;
		}
	}

	if (nDeltaX < 0) {
		nDirection = -nDirection & 7;
	}
	if (nDeltaY > 0) {
		nDirection = g_GAME_PositiveYDirectionMap[nDirection];
	}
	return nDirection;
}
