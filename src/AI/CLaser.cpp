#include "AI/CLaser.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_LINKSCF_LasrChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetLasrChunkObjectRuntimeStateThunk(void* pObject);

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

// FUNCTION: LEMBALL 0x00428890
CLaser::CLaser(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x2f, 0, 0);
	*(void**) this = g_LINKSCF_LasrChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x004288b0
void CLaser::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	ResetLasrChunkObjectRuntimeStateThunk(this);
}

// FUNCTION: LEMBALL 0x004288d0
void CLaser::Initialise(void)
{
	char* pObjectBytes;
	int nZero;

	pObjectBytes = (char*) this;
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	nZero = 0;
	*(int*) (pObjectBytes + 0x94) = nZero;
	*(int*) (pObjectBytes + 0x138) = nZero;
	*(int*) (pObjectBytes + 0x13c) = nZero;
}

// FUNCTION: LEMBALL 0x004288f0
CLaser::~CLaser(void)
{
	*(void**) this = g_LINKSCF_LasrChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject)
{
	((CLaser*) pObject)->~CLaser();
}
