#include "ENGINE/CORE/VSINIT.H"

void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

// Split from LEVELVT.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x0040c260
void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseC260(void* pObject)
{
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}
