#include "Platform/Windows/Mixed/Engine/GDI/VSGDI.H"

class HelperLocalRectView {
public:
	void UpdateFromPackagedEntry(char* pEntry);
};

// FUNCTION: LEMBALL 0x0046cbe0
void HelperLocalRectView::UpdateFromPackagedEntry(char* pEntry)
{
	char* pThis;
	char* pBase;
	char* pTarget;
	char* pTargetRect;
	short* pRect;
	short* pPoint;
	short nClipX;
	short nClipY;
	short nClipWidth;
	short nClipHeight;
	int nOffset;

	pThis = (char*) this;
	nOffset = *(int*) (*(int*) (pThis - 0x51c) + 4);
	pBase = pThis + nOffset;
	pRect = (short*) (pBase - 0x4f8);
	if ((*(unsigned char*) (pEntry + 0x0d) & 0x10) == 0) {
		pRect[0] = *(short*) (pEntry + 4);
		pRect[1] = *(short*) (pEntry + 6);
		pPoint = (short*) (pEntry + 8);
		if (pEntry == (char*) -4) {
			pPoint = 0;
		}
		pRect[2] = pPoint[0];
		pRect[3] = pPoint[1];
	} else if (*(short*) (pEntry + 4) * *(short*) (pEntry + 6) != 0) {
		if (*(short*) (pEntry + 8) < pRect[2]) {
			pRect[0] += pRect[2] - *(short*) (pEntry + 8);
			pRect[2] = *(short*) (pEntry + 8);
		}
		if ((short) (pRect[0] + pRect[2]) <
			(short) (*(short*) (pEntry + 8) + *(short*) (pEntry + 4))) {
			pRect[0] = *(short*) (pEntry + 4) - pRect[2] + *(short*) (pEntry + 8);
		}
		if (*(short*) (pEntry + 10) < pRect[3]) {
			pRect[1] += pRect[3] - *(short*) (pEntry + 10);
			pRect[3] = *(short*) (pEntry + 10);
		}
		if ((short) (pRect[1] + pRect[3]) <
			(short) (*(short*) (pEntry + 10) + *(short*) (pEntry + 6))) {
			pRect[1] = *(short*) (pEntry + 6) - pRect[3] + *(short*) (pEntry + 10);
		}
	}

	pTarget = *(char**) (pBase - 0x4fc);
	if (pTarget != (char*) g_pResourceGeometryHelperTarget &&
		(*(unsigned char*) (pEntry + 0x0e) & 1) == 0) {
		nOffset = *(int*) (*(int*) (pTarget + 0x40) + 4);
		pTargetRect = pTarget + nOffset + 0x64;
		nClipWidth = *(short*) (pTargetRect + 0);
		nClipHeight = *(short*) (pTargetRect + 2);
		nClipX = *(short*) (pTargetRect + 4);
		nClipY = *(short*) (pTargetRect + 6);
		if (pRect[2] < nClipX) {
			pRect[0] += pRect[2] - nClipX;
			pRect[2] = nClipX;
		}
		if ((short) (nClipX + nClipWidth) < (short) (pRect[2] + pRect[0])) {
			pRect[0] = nClipWidth - pRect[2] + nClipX;
		}
		if (pRect[3] < nClipY) {
			pRect[1] += pRect[3] - nClipY;
			pRect[3] = nClipY;
		}
		if ((short) (nClipY + nClipHeight) < (short) (pRect[3] + pRect[1])) {
			pRect[1] = nClipHeight - pRect[3] + nClipY;
		}
		if (pRect[0] < 1 || pRect[1] < 1) {
			pRect[0] = 0;
			pRect[1] = 0;
			pRect[2] = 0;
			pRect[3] = 0;
		}
	}
}
