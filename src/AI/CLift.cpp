#include "AI/CLift.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_LINKSCF_LiftChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

// FUNCTION: LEMBALL 0x00424d00
CLift::CLift(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x212, 0, 0);
	*(void**) this = g_LINKSCF_LiftChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x00424d20
CLift::~CLift(void)
{
	*(void**) this = g_LINKSCF_LiftChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}
