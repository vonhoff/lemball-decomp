#include "AI/CInvisibleSwitch.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);
extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;
struct InvisibleSwitchVtableLayout;
extern InvisibleSwitchVtableLayout g_LINKSCF_InvsChunkObjectVtable;
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

struct ManagedEntitySlotOwnerView {
	void SetManagedEntitySlotId(unsigned short nSlotId);
};

struct LevelSlotActionDispatcherView {
	void DispatchLevelSlotActionByType(int nAction, unsigned int nSlotId, int nRangeEnd, int nUnused4);
};

struct InvsChunkObjectActionView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateId(int nStateId);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
};

struct InvsChunkActionRecord {
	int m_nAction;
	unsigned short m_nSlotId;
	unsigned short m_nReserved;
};

// Split from the original LINKSCF source group to preserve MSVC 4.00 code generation in LINKSCF.CPP.

// FUNCTION: LEMBALL 0x00409ca0
CInvisibleSwitch::CInvisibleSwitch(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x36, 0, 0);
	*(void**) this = &g_LINKSCF_InvsChunkObjectVtable;
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
	*(void**) this = &g_LINKSCF_InvsChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x00409d70
void CInvisibleSwitch::Set(const tCoord3d& begin, const tCoord3d& end)
{
	char* pObjectBytes = (char*) this;
	short nSwap;
	int nX;
	int nY;

	*(int*) (pObjectBytes + 0x254) = 0;
	*(tCoord3d*) (pObjectBytes + 0x138) = begin;
	*(tCoord3d*) (pObjectBytes + 0x13e) = end;
	if (*(short*) (pObjectBytes + 0x138) > *(short*) (pObjectBytes + 0x13e)) {
		nSwap = *(short*) (pObjectBytes + 0x138);
		*(short*) (pObjectBytes + 0x138) = *(short*) (pObjectBytes + 0x13e);
		*(short*) (pObjectBytes + 0x13e) = nSwap;
	}
	if (*(short*) (pObjectBytes + 0x13a) > *(short*) (pObjectBytes + 0x140)) {
		nSwap = *(short*) (pObjectBytes + 0x13a);
		*(short*) (pObjectBytes + 0x13a) = *(short*) (pObjectBytes + 0x140);
		*(short*) (pObjectBytes + 0x140) = nSwap;
	}
	nY = *(short*) (pObjectBytes + 0x13a);
	*(int*) (pObjectBytes + 0x144) = 0;
	*(int*) (pObjectBytes + 0x9c) = *(short*) (pObjectBytes + 0x138) << 12;
	*(int*) (pObjectBytes + 0x148) = 0;
	*(int*) (pObjectBytes + 0xa0) = nY << 12;
	*(int*) (pObjectBytes + 0xa4) = *(short*) (pObjectBytes + 0x13c) << 12;

	if (nY <= *(short*) (pObjectBytes + 0x140)) {
		do {
			nX = *(short*) (pObjectBytes + 0x138);
			if (nX <= *(short*) (pObjectBytes + 0x13e)) {
				do {
					int nTileX = nX / 16;
					if (nTileX >= 0) {
						int nTileY = nY / 16;
						if (nTileY >= 0 && nTileX < *(int*) ((char*) g_pLevelTileGrid + 0x10) &&
							nTileY < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
							char* pGrid = (char*) g_pLevelTileGrid;
							*(unsigned char*) (*(char**) (pGrid + 0x0c) +
											   (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12 + 7) |= 0x80;
						}
					}
					nX += 16;
				} while (nX <= *(short*) (pObjectBytes + 0x13e));
			}
			nY += 16;
		} while (nY <= *(short*) (pObjectBytes + 0x140));
	}
	*(unsigned short*) (pObjectBytes + 0x150) = 0;
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

// FUNCTION: LEMBALL 0x0040a050
int CInvisibleSwitch::Process(void)
{
	char* pObjectBytes = (char*) this;
	int nStateId;

	VerifyObjects();
	if (*(int*) (pObjectBytes + 0x114) != 0) {
		nStateId = *(int*) (pObjectBytes + 0xb8);
		if (*(int*) (pObjectBytes + 0x128) != nStateId && nStateId != 0x1a) {
			return 1;
		}
	}

	if (*(int*) (pObjectBytes + 0xb8) == 0x1a) {
		if (*(int*) (pObjectBytes + 0x144) == 0) {
			*(int*) (pObjectBytes + 0x148) = 1;
		}
		if (*(void**) (pObjectBytes + 0x5c) != 0) {
			AddObject((CGameObject*) *(void**) (pObjectBytes + 0x5c));
			*(void**) (pObjectBytes + 0x5c) = 0;
		}
		{
			InvsChunkActionRecord* pAction;
			int iAction = 0;
			if (*(unsigned short*) (pObjectBytes + 0x150) > iAction) {
				pAction = (InvsChunkActionRecord*) (pObjectBytes + 0x154);
				do {
					((LevelSlotActionDispatcherView*) g_pActiveManagedEntityOwner)
						->DispatchLevelSlotActionByType(pAction->m_nAction, pAction->m_nSlotId, 0, 0);
					++pAction;
					++iAction;
				} while (*(unsigned short*) (pObjectBytes + 0x150) > iAction);
			}
		}
		((InvsChunkObjectActionView*) this)->SetActionState(0x15);
		((InvsChunkObjectActionView*) this)->SetManagedEntityStateId(0x18);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0040a110
void CInvisibleSwitch::Load(unsigned char*& pData)
{
	tCoord3d begin;
	tCoord3d end;
	char* pObjectBytes = (char*) this;
	InvsChunkActionRecord* pAction;
	int iAction;
	unsigned short nSlotId;

	nSlotId = *(unsigned short*) pData;
	pData += 2;
	((ManagedEntitySlotOwnerView*) this)->SetManagedEntitySlotId(nSlotId);
	begin.x = *(short*) pData;
	pData += 2;
	begin.y = *(short*) pData;
	pData += 2;
	begin.z = *(short*) pData;
	pData += 2;
	end.x = *(short*) pData;
	pData += 2;
	end.y = *(short*) pData;
	pData += 2;
	end.z = *(short*) pData;
	pData += 2;
	Set(begin, end);

	if (*(unsigned short*) ((char*) g_pActiveManagedEntityOwner + 0x54) >= 9) {
		*(int*) (pObjectBytes + 0x144) = *(unsigned short*) pData;
		pData += 2;
	}
	else {
		*(int*) (pObjectBytes + 0x144) = 0;
	}

	*(unsigned short*) (pObjectBytes + 0x150) = *(unsigned short*) pData;
	pData += 2;
	iAction = 0;
	if (*(unsigned short*) (pObjectBytes + 0x150) > iAction) {
		pAction = (InvsChunkActionRecord*) (pObjectBytes + 0x154);
		do {
			pAction->m_nAction = *(unsigned short*) pData;
			pData += 2;
			pAction->m_nSlotId = *(unsigned short*) pData;
			pData += 2;
			++pAction;
			++iAction;
		} while (iAction < *(unsigned short*) (pObjectBytes + 0x150));
	}
}
