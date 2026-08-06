#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

// Split from LEVELVT.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x0040c260
void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseC260(void* pObject)
{
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x0040eca0
void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseECA0(void* pObject)
{
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x00424cf0
void LEMBALL_FASTCALL destroy_level_chunk_object_base24cf0(void* pObject)
{
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}
