#include "AI/CHand.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern void* g_LINKSCF_CaptureChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

// FUNCTION: LEMBALL 0x00427ad0
CHand::CHand(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x1f, 0, 0);
	*(void**) this = g_LINKSCF_CaptureChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x00427af0
void CHand::Initialise(void)
{
	int nZero;

	nZero = 0;
	*(int*) ((char*) this + 0x94) = nZero;
	*(int*) ((char*) this + 0x138) = nZero;
	*(int*) ((char*) this + 0x13c) = nZero;
}

// FUNCTION: LEMBALL 0x00427b10
void CHand::Restart(void)
{
	char* pObjectBytes;
	int nX;
	int nY;
	int nZ;

	ResetManagedEntityRuntimeStateThunk(this);
	Initialise();
	pObjectBytes = (char*) this;
	nZ = *(int*) (pObjectBytes + 0x48);
	nX = *(int*) (pObjectBytes + 0x40);
	nY = *(int*) (pObjectBytes + 0x44);
	*(int*) (pObjectBytes + 0x9c) = nX;
	*(int*) (pObjectBytes + 0xa0) = nY;
	*(int*) (pObjectBytes + 0xa4) = nZ;
}

// FUNCTION: LEMBALL 0x00427b40
CHand::~CHand(void)
{
	*(void**) this = g_LINKSCF_CaptureChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}
