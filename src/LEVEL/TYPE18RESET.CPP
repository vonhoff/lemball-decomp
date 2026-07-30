#include "ENGINE/CORE/COMMON.H"

extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;

extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

// FUNCTION: LEMBALL 0x0040c350
void LEMBALL_FASTCALL ResetType18ChunkObjectRuntimeState(void* pObject)
{
	char* pObjectBytes;
	int y;
	int z;
	int x;
	int nTick;

	pObjectBytes = (char*) pObject;
	ResetManagedEntityRuntimeStateThunk(pObject);
	x = *(int*) (pObjectBytes + 0x40);
	y = *(int*) (pObjectBytes + 0x44);
	z = *(int*) (pObjectBytes + 0x48);
	*(int*) (pObjectBytes + 0x9c) = x;
	*(int*) (pObjectBytes + 0xa0) = y;
	*(int*) (pObjectBytes + 0xa4) = z;
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	*(int*) (pObjectBytes + 0x94) = g_nLevelFrameClockTimeMs;
	nTick = g_nLevelFrameClockTick;
	*(int*) (pObjectBytes + 0x148) = 1;
	*(int*) (pObjectBytes + 0x14c) = 0x50;
	*(int*) (pObjectBytes + 0xcc) = nTick;
}
