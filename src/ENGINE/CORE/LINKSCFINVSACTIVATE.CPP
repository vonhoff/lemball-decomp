#include "ENGINE/CORE/COMMON.H"

typedef void(LEMBALL_FASTCALL* InvsChunkObjectActivateProc)(void* pObject);

// FUNCTION: LEMBALL 0x0040a3b0
void LEMBALL_FASTCALL AdvanceInvsChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	i = 0;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nOffset = 0;
		do {
			pChunkObject = *(char**) (pManagerBytes + 0x3c) + nOffset;
			++i;
			nOffset += 0x2b8;
			((InvsChunkObjectActivateProc) (*(void***) pChunkObject)[5])(pChunkObject);
		} while (*(int*) (pManagerBytes + 0x34) > i);
	}
}
