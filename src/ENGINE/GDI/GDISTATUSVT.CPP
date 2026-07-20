#include "ENGINE/CORE/VSMEM.H"

extern void LEMBALL_FASTCALL DestroyArrowCursorState(void* pRenderClient);

// FUNCTION: LEMBALL 0x0046b460
int LEMBALL_FASTCALL DispatchStatusIndicatorRenderEntry(void* pRenderClient, int nUnused, void* pEntry)
{
	short* pEntryWords = (short*) pEntry;
	short width = pEntryWords[4];
	short height = pEntryWords[5];
	short x = pEntryWords[6];
	short y = pEntryWords[7];
	short clipWidth = pEntryWords[8];
	short clipHeight = pEntryWords[9];
	short* pClipOrigin;

	(void) nUnused;
	pClipOrigin = pEntry == (void*) -8 ? 0 : pEntryWords + 6;
	if (pClipOrigin != 0) {
		x = pClipOrigin[0];
		y = pClipOrigin[1];
	}
	pClipOrigin = pEntry == (void*) -16 ? 0 : pEntryWords + 10;
	if (clipWidth * clipHeight != 0 && pClipOrigin != 0) {
		short left = (short) (pClipOrigin[0] + x);
		short top = (short) (pClipOrigin[1] + y);
		if (x < left) {
			width = (short) (width + x - left);
			x = left;
		}
		if ((short) (left + clipWidth) < (short) (x + width)) {
			width = (short) (left - x + clipWidth);
		}
		if (y < top) {
			height = (short) (height + y - top);
			y = top;
		}
		if ((short) (top + clipHeight) < (short) (y + height)) {
			height = (short) (top - y + clipHeight);
		}
		if (width < 1 || height < 1) {
			x = y = width = height = 0;
		}
	}
	return x <= *(short*) ((char*) pRenderClient + 0x10) &&
		   *(short*) ((char*) pRenderClient + 0x10) < (short) (x + width) &&
		   y <= *(short*) ((char*) pRenderClient + 0x12) &&
		   *(short*) ((char*) pRenderClient + 0x12) < (short) (y + height);
}

// FUNCTION: LEMBALL 0x0046ba20
void LEMBALL_FASTCALL NoopStatusIndicatorRenderClientSlot(void* pRenderClient)
{
	(void) pRenderClient;
}

// FUNCTION: LEMBALL 0x0046ba30
void* LEMBALL_FASTCALL DeleteStatusIndicatorRenderClientBaseThunk(void* pRenderClient,
																  int nUnused,
																  unsigned char fDelete)
{
	(void) nUnused;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pRenderClient);
	}
	return pRenderClient;
}

// FUNCTION: LEMBALL 0x0046dad0
void* LEMBALL_FASTCALL DeleteArrowCursorRenderClientThunk(void* pRenderClient, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	DestroyArrowCursorState(pRenderClient);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pRenderClient);
	}
	return pRenderClient;
}
