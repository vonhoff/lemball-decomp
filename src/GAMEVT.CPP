#include "GAME.H"

extern void* RestoreLevelDemoPlaybackBaseVtable(void* pPlaybackController, unsigned char fDelete);

// FUNCTION: LEMBALL 0x0040298c
void* LEMBALL_FASTCALL DeleteRenderQueueNodeThunk(void* pObject, int nUnused, unsigned char fDelete)
{
	(void) nUnused;
	return RestoreLevelDemoPlaybackBaseVtable(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00462ec0
int LEMBALL_FASTCALL IncrementRenderQueueNodeCounterThunk(void* pRenderQueueNode, int nUnused, void* pEntry)
{
	(void) nUnused;
	(void) pEntry;
	++*(int*) ((char*) pRenderQueueNode + 0x0c);
	return 0;
}
