#include "ENGINE/CORE/COMMON.H"

// FUNCTION: LEMBALL 0x00451fd0
void LEMBALL_FASTCALL MarkPasswordEntryRenderContextDirty(void* pContext)
{
	int nRenderContext;
	int nRenderObject;

	nRenderContext = *(int*) (*(int*) ((char*) pContext + 0x4c) + 0x0c);
	nRenderObject = nRenderContext + *(int*) (*(int*) (nRenderContext + 0x40) + 4) + 0x74;
	*(int*) nRenderObject = 1;
}
