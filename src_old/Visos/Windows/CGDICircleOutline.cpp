#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

// FUNCTION: LEMBALL 0x00475ce0
void LEMBALL_FASTCALL plot_circle_outline_quadrants_clipped(
	void* pThis, int nUnusedEdx, int nX, int nY, int nRadius, unsigned char nPixel)
{
	char* pActive;
	char** ppRows;
	int nActiveOffset;
	int nClipX;
	int nClipY;
	int nClipRight;
	int nClipBottom;
	int nXOffset;
	int nError;
	int nStep;
	int nDelta;
	int nRowTopOffset;
	int nRowBottomOffset;

	(void) nUnusedEdx;
	nXOffset = 0;
	nError = 0;
	nStep = 1;
	nDelta = nRadius * 2 - 1;
	ppRows = *(char***) ((char*) pThis + 4);

	nActiveOffset = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	pActive = (char*) pThis + nActiveOffset;
	nClipX = *(short*) (pActive + 0x68);
	nClipRight = nClipX + *(short*) (pActive + 0x64) - 1;
	if (nClipX <= nX && nX <= nClipRight) {
		nClipY = *(short*) (pActive + 0x6a);
		if (nClipY <= nY + nRadius && nY + nRadius <= nClipY + *(short*) (pActive + 0x66) - 1) {
			ppRows[nY + nRadius][nX] = nPixel;
		}
	}

	nActiveOffset = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	pActive = (char*) pThis + nActiveOffset;
	nClipX = *(short*) (pActive + 0x68);
	nClipRight = nClipX + *(short*) (pActive + 0x64) - 1;
	if (nClipX <= nX && nX <= nClipRight) {
		nClipY = *(short*) (pActive + 0x6a);
		if (nClipY <= nY - nRadius && nY - nRadius <= nClipY + *(short*) (pActive + 0x66) - 1) {
			ppRows[nY - nRadius][nX] = nPixel;
		}
	}

	nActiveOffset = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	pActive = (char*) pThis + nActiveOffset;
	nClipX = *(short*) (pActive + 0x68);
	nClipRight = nClipX + *(short*) (pActive + 0x64) - 1;
	nClipY = *(short*) (pActive + 0x6a);
	nClipBottom = nClipY + *(short*) (pActive + 0x66) - 1;
	if (nClipX <= nX + nRadius && nX + nRadius <= nClipRight && nClipY <= nY && nY <= nClipBottom) {
		ppRows[nY][nX + nRadius] = nPixel;
	}

	nActiveOffset = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	pActive = (char*) pThis + nActiveOffset;
	nClipX = *(short*) (pActive + 0x68);
	nClipRight = nClipX + *(short*) (pActive + 0x64) - 1;
	nClipY = *(short*) (pActive + 0x6a);
	nClipBottom = nClipY + *(short*) (pActive + 0x66) - 1;
	if (nClipX <= nX - nRadius && nX - nRadius <= nClipRight && nClipY <= nY && nY <= nClipBottom) {
		ppRows[nY][nX - nRadius] = nPixel;
	}

	if (nRadius > 0) {
		nRowTopOffset = (nY - nRadius) * 4;
		nRowBottomOffset = (nY + nRadius) * 4;
		do {
			++nXOffset;
			nError += nStep;
			if (nDelta < nError * 2) {
				--nRadius;
				nRowTopOffset += 4;
				nRowBottomOffset -= 4;
				nError -= nDelta;
				nDelta -= 2;
			}
			if (nXOffset <= nRadius) {
				if (((int (LEMBALL_FASTCALL*)(void*, int, int, int)) 0x00475f60)(
						pThis, 0, nX + nXOffset, nY + nRadius)) {
					(*(char***) ((char*) pThis + 4))[nY + nRadius][nX + nXOffset] = nPixel;
				}
				if (((int (LEMBALL_FASTCALL*)(void*, int, int, int)) 0x00475f60)(
						pThis, 0, nX - nXOffset, nY + nRadius)) {
					(*(char***) ((char*) pThis + 4))[nY + nRadius][nX - nXOffset] = nPixel;
				}
				if (((int (LEMBALL_FASTCALL*)(void*, int, int, int)) 0x00475f60)(
						pThis, 0, nX + nXOffset, nY - nRadius)) {
					(*(char***) ((char*) pThis + 4))[nY - nRadius][nX + nXOffset] = nPixel;
				}
				if (((int (LEMBALL_FASTCALL*)(void*, int, int, int)) 0x00475f60)(
						pThis, 0, nX - nXOffset, nY - nRadius)) {
					(*(char***) ((char*) pThis + 4))[nY - nRadius][nX - nXOffset] = nPixel;
				}
				if (nXOffset < nRadius) {
					((void (LEMBALL_FASTCALL*)(void*, int, int, int, int, int, unsigned char)) 0x00475fb0)(
						pThis, 0, nX, nY, nRadius, nXOffset, nPixel);
				}
			}
			nStep += 2;
		} while (nXOffset < nRadius);
	}
}
