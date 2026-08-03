#include "AI/CInvisibleSwitch.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);
extern void* g_pActiveManagedEntityOwner;
extern void* g_LINKSCF_InvsChunkObjectVtable[16];
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetInvsChunkObjectStateThunk(void* pObject);

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

struct LevelVtSmallFunctionView {
	void AddLevelScoreClamped(int nValue);
};

struct InvsChunkObjectSoundState {
	unsigned char m_abReserved00[0x14c];
	int m_fSoundTriggered14C;
};

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

// Split from the original LINKSCF source group to preserve MSVC 4.00 code generation in LINKSCF.CPP.

// FUNCTION: LEMBALL 0x00409ca0
CInvisibleSwitch::CInvisibleSwitch(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x36, 0, 0);
	*(void**) this = g_LINKSCF_InvsChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x00409cc0
void CInvisibleSwitch::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	ResetInvsChunkObjectStateThunk(this);
}

// FUNCTION: LEMBALL 0x00409ce0
void CInvisibleSwitch::Initialise(void)
{
	char* pObjectBytes = (char*) this;
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	*(unsigned short*) (pObjectBytes + 0x150) = 0;
	*(int*) (pObjectBytes + 0x148) = 0;
	*(int*) (pObjectBytes + 0x254) = 0;
	*(int*) (pObjectBytes + 0x144) = 0;
	*(int*) (pObjectBytes + 0x5c) = 0;
	*(int*) (pObjectBytes + 0x14c) = 0;
}

// FUNCTION: LEMBALL 0x00409d10
CInvisibleSwitch::~CInvisibleSwitch(void)
{
	*(void**) this = g_LINKSCF_InvsChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x00409ec0
void CInvisibleSwitch::VerifyObjects(void)
{
	char* pObjectBytes = (char*) this;
	int iEntity = 0;
	while (iEntity < *(int*) (pObjectBytes + 0x254)) {
		char* pEntity = *(char**) (pObjectBytes + 0x258 + iEntity * 4);
		int nX = *(int*) (pEntity + 0x9c) >> 12;
		int nY = *(int*) (pEntity + 0xa0) >> 12;
		if (nX < *(short*) (pObjectBytes + 0x138) - 8 || *(short*) (pObjectBytes + 0x13e) + 7 < nX ||
			nY < *(short*) (pObjectBytes + 0x13a) - 8 || *(short*) (pObjectBytes + 0x140) + 7 < nY) {
			int iMove;
			*(unsigned short*) (pEntity + 0x120) = 0xffff;
			for (iMove = iEntity + 1; iMove < *(int*) (pObjectBytes + 0x254); ++iMove) {
				*(void**) (pObjectBytes + 0x258 + (iMove - 1) * 4) = *(void**) (pObjectBytes + 0x258 + iMove * 4);
			}
			--*(int*) (pObjectBytes + 0x254);
			continue;
		}
		++iEntity;
	}
}

// FUNCTION: LEMBALL 0x00409f70
void CInvisibleSwitch::AddObject(CGameObject* pEntity)
{
	char* pObjectBytes = (char*) this;
	int cEntities = *(int*) (pObjectBytes + 0x254);
	if (cEntities < 0x18) {
		*(CGameObject**) (pObjectBytes + 0x258 + cEntities * 4) = pEntity;
		++*(int*) (pObjectBytes + 0x254);
		*(unsigned short*) ((char*) pEntity + 0x120) = GetManagedEntitySlotIdThunk((int) (unsigned long) this);
	}
}

// FUNCTION: LEMBALL 0x00409fa0
void CInvisibleSwitch::StepOn(const AICOORD& position, CGameObject* pEntity)
{
	char* pObjectBytes = (char*) this;
	char* pEntityBytes = (char*) pEntity;
	int nX;
	int nY;

	if (*(int*) (pObjectBytes + 0x148) != 0 || *(int*) (pObjectBytes + 0x12c) != 0x18 ||
		GetManagedEntitySlotIdThunk((int) (unsigned long) this) == *(unsigned short*) (pEntityBytes + 0x120)) {
		return;
	}
	nX = position.x >> 12;
	nY = position.y >> 12;
	if (nX < *(short*) (pObjectBytes + 0x138) - 8 || *(short*) (pObjectBytes + 0x13e) + 7 < nX ||
		nY < *(short*) (pObjectBytes + 0x13a) - 8 || *(short*) (pObjectBytes + 0x140) + 7 < nY) {
		return;
	}
	*(CGameObject**) (pObjectBytes + 0x5c) = pEntity;
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
}

// FUNCTION: LEMBALL 0x0040a020
void CInvisibleSwitch::DoActivate(void)
{
	InvsChunkObjectSoundState* pState = (InvsChunkObjectSoundState*) this;
	if (pState->m_fSoundTriggered14C == 0) {
		((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0x32);
		pState->m_fSoundTriggered14C = 1;
	}
}
