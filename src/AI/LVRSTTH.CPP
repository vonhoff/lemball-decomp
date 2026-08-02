#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

// Split from LEVELRUN.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x0040d490
void LEMBALL_FASTCALL ResetManagedEntityBaseRuntimeState(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
}
