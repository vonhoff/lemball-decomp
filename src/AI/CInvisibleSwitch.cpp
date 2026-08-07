#include "AI/CInvisibleSwitch.h"

extern int g_nLevelFrameClockTick;
extern void* g_pLevelProgressState;
extern void* g_pSessionRandomState;
extern int g_nSelectedNetworkLobbyPeerId;

#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);
extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;
struct InvisibleSwitchVtableLayout;
extern InvisibleSwitchVtableLayout g_LINKSCF_InvsChunkObjectVtable;
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetInvsChunkObjectStateThunk(void* pObject);

struct CInvsTileGrid {
	char m_abReserved00[0xc];
	unsigned char* m_pTileData0C;   // 0x0c
	int m_nWidth10;                  // 0x10
	int m_nHeight14;                 // 0x14
};

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
	m_nStateB8 = 0x18;
	*(unsigned short*) (pObjectBytes + 0x150) = 0;
	m_nActive148 = 0;
	*(int*) (pObjectBytes + 0x254) = 0;
	m_nTriggered144 = 0;
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
	m_RangeStart138 = begin;
	m_RangeEnd13E = end;
	if (m_RangeStart138.x > m_RangeEnd13E.x) {
		nSwap = m_RangeStart138.x;
		m_RangeStart138.x = m_RangeEnd13E.x;
		m_RangeEnd13E.x = nSwap;
	}
	if (m_RangeStart138.y > m_RangeEnd13E.y) {
		nSwap = m_RangeStart138.y;
		m_RangeStart138.y = m_RangeEnd13E.y;
		m_RangeEnd13E.y = nSwap;
	}
	nY = m_RangeStart138.y;
	m_nTriggered144 = 0;
	m_WorldPosition9C.x = m_RangeStart138.x << 12;
	m_nActive148 = 0;
	m_WorldPosition9C.y = nY << 12;
	m_WorldPosition9C.z = m_RangeStart138.z << 12;

	if (nY <= m_RangeEnd13E.y) {
		do {
			nX = m_RangeStart138.x;
			if (nX <= m_RangeEnd13E.x) {
				do {
					int nTileX = nX / 16;
					if (nTileX >= 0) {
						int nTileY = nY / 16;
						if (nTileY >= 0) {
							CInvsTileGrid* pGrid = (CInvsTileGrid*) g_pLevelTileGrid;
							if (nTileX < pGrid->m_nWidth10 && nTileY < pGrid->m_nHeight14) {
								pGrid->m_pTileData0C[(nTileY * pGrid->m_nWidth10 + nTileX) * 12 + 7] |= 0x80;
							}
						}
					}
					nX += 16;
				} while (nX <= m_RangeEnd13E.x);
			}
			nY += 16;
		} while (nY <= m_RangeEnd13E.y);
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
		if (nX < m_RangeStart138.x - 8 || m_RangeEnd13E.x + 7 < nX ||
			nY < m_RangeStart138.y - 8 || m_RangeEnd13E.y + 7 < nY) {
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

	if (m_nActive148 != 0 || m_anRuntimeState12C[0] != 0x18 ||
		GetManagedEntitySlotIdThunk((int) (unsigned long) this) == *(unsigned short*) (pEntityBytes + 0x120)) {
		return;
	}
	nX = position.x >> 12;
	nY = position.y >> 12;
	if (nX < m_RangeStart138.x - 8 || m_RangeEnd13E.x + 7 < nX ||
		nY < m_RangeStart138.y - 8 || m_RangeEnd13E.y + 7 < nY) {
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
	if (m_nPendingState114 != 0) {
		nStateId = m_nStateB8;
		if (m_nLastState128 != nStateId && nStateId != 0x1a) {
			return 1;
		}
	}

	if (m_nStateB8 == 0x1a) {
		if (m_nTriggered144 == 0) {
			m_nActive148 = 1;
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
	((CGameObject*) this)->SetId(nSlotId);
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
		m_nTriggered144 = *(unsigned short*) pData;
		pData += 2;
	}
	else {
		m_nTriggered144 = 0;
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

// MACINTOSH: CSheep::HitBall()
// FUNCTION: LEMBALL 0x0041fab0
void LEMBALL_FASTCALL HandleShpgChunkObjectProjectileHit(void)
{
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0x96);
}

// MACINTOSH: CEnemy::HitBall()
// FUNCTION: LEMBALL 0x004206f0
void LEMBALL_FASTCALL HandleEnmyChunkObjectProjectileHit(int pObject)
{
	*(int*) ((char*) pObject + 0x124) = 1;
	*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick + 0x3c;
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(300);
}

// MACINTOSH: CEnemy::GetHit()
// FUNCTION: LEMBALL 0x00420720
void LEMBALL_FASTCALL RemoveEnemyAward500(int pObject)
{
	int* pCount;
	int nIndex;
	int* pArray;
	int** ppArray;

	pCount = (int*) ((char*) g_pActiveManagedEntityOwner + 0x118);
	nIndex = 0;
	if (*pCount > 0) {
		pArray = *(int**) ((char*) g_pActiveManagedEntityOwner + 0x120);
		ppArray = (int**) ((char*) g_pActiveManagedEntityOwner + 0x120);
		do {
			if (*pArray == pObject) {
				*pCount = *pCount - 1;
				while (nIndex < *pCount) {
					(*ppArray)[nIndex] = (*ppArray)[nIndex + 1];
					++nIndex;
				}
				break;
			}
			pArray = pArray + 1;
			++nIndex;
		} while (nIndex < *pCount);
	}
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(500);
}

// MACINTOSH: CBall::Delete()
// FUNCTION: LEMBALL 0x00421ab0
void LEMBALL_FASTCALL RemoveBallChunkEntryFromLevelLists(int pObject)
{
	int* pCount;
	int nIndex;
	int** ppArray;
	int nCount;
	int* pArray;

	nIndex = 0;
	nCount = *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118);
	pCount = (int*) ((char*) g_pActiveManagedEntityOwner + 0x118);
	if (nCount > 0) {
		pArray = *(int**) ((char*) g_pActiveManagedEntityOwner + 0x120);
		ppArray = (int**) ((char*) g_pActiveManagedEntityOwner + 0x120);
		do {
			if ((void*) *pArray == (void*) pObject) {
				*pCount = nCount - 1;
				if (nIndex < nCount - 1) {
					nCount = nIndex * 4;
					do {
						nIndex = nIndex + 1;
						*(int*) ((char*) *ppArray + nCount) = *(int*) ((char*) *ppArray + nCount + 4);
						nCount = nCount + 4;
					} while (nIndex < *pCount);
				}
				*(int*) ((char*) *ppArray + *pCount * 4) = 0;
				break;
			}
			pArray = pArray + 1;
			nIndex = nIndex + 1;
		} while (nIndex < nCount);
	}
	((void(__fastcall*)(void*, int)) 0x40302b)((void*) *(int*) 0x49e1bc, pObject);
	((CGameObject*) pObject)->SetId(0xffff);
}

// MACINTOSH: CPlayerLemming::HitBullet()
// FUNCTION: LEMBALL 0x0040f000
void LEMBALL_FASTCALL HandleDoorChunkObjectTargetTileProjectileResult(int pObject, int pTarget)
{
	int iState;

	if (*(int*) ((char*) g_pLevelProgressState + 0x20) == 0) {
		iState = *(int*) ((char*) pObject + 0xb8);
		if (iState < 4 || (5 < iState && iState != 0x10)) {
			if (*(int*) ((char*) pTarget + 0x16c) == 1) {
				unsigned int uRandom;
				uRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1f;
				*(unsigned int*) g_pSessionRandomState = uRandom & 0x7fffff;
				if ((uRandom & 1) != 0) {
					return;
				}
			}
			else {
				if (*(int*) ((char*) pTarget + 0x16c) != 2) {
					return;
				}
				if (g_nSelectedNetworkLobbyPeerId != 0) {
					((void(__fastcall*)(void*, void*)) 0x402895)((void*) *(int*) 0x49d12c, (void*) pTarget);
				}
			}
			*(int*) ((char*) pObject + 0xc0) = 0;
			*(int*) ((char*) pObject + 0x178) = 1;
			*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick + 0x28;
			*(unsigned short*) ((char*) pObject + 0xb4) = *(short*) ((char*) pTarget + 0xb4) + 4U & 7;
		}
	}
}

// MACINTOSH: CDuplicator::CDuplicator() [type-0x1c tile-flag chunk]
// FUNCTION: LEMBALL 0x004275b0
void* LEMBALL_FASTCALL ConstructTileFlagChunkObject(void* pObject, int nUnused, void* pOrigin)
{
	((CGameObject*) pObject)->InitializeLevelChunkObjectBase(0x1c, 0, 0);
	(* (void***) pObject) = (void**) 0x496050;
	*(int*) ((char*) pObject + 0x40) = *(int*) pOrigin;
	*(int*) ((char*) pObject + 0x44) = *(int*) ((char*) pOrigin + 4);
	*(int*) ((char*) pObject + 0x48) = *(int*) ((char*) pOrigin + 8);
	return pObject;
}

// MACINTOSH: construct_boon_chunk_object(int, int)
// FUNCTION: LEMBALL 0x0042a5c0
void* LEMBALL_FASTCALL ConstructBoonChunkObject(void* pObject, int nEntityType, int nValue)
{
	((CGameObject*) pObject)->ConstructCGameObject(nEntityType, 0, 10);
	(* (void***) pObject) = (void**) 0x496488;
	*(int*) ((char*) pObject + 0x124) = nValue;
	return pObject;
}

// MACINTOSH: CBall::CBall() [type-9 ball chunk]
// FUNCTION: LEMBALL 0x00421660
void* LEMBALL_FASTCALL ConstructBallChunkEntry(void* pObject)
{
	((CGameObject*) pObject)->ConstructCGameObject(9, 0, 0);
	(* (void***) pObject) = (void**) 0x495460;
	*(int*) ((char*) pObject + 0x124) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x128) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x12c) = 0xaa55aa55;
	return pObject;
}

// MACINTOSH: CCollectable::CCollectable() [type-10 coll chunk]
// FUNCTION: LEMBALL 0x00422870
void* LEMBALL_FASTCALL ConstructCollChunkObject(void* pObject, int nUnused, int nWorldX, int nWorldY, int nWorldZ, int nEntityType)
{
	((CGameObject*) pObject)->InitializeLevelChunkObjectBase(nEntityType, 0, 0);
	(* (void***) pObject) = (void**) 0x4959c0;
	*(int*) ((char*) pObject + 0x40) = nWorldX << 12;
	*(int*) ((char*) pObject + 0x44) = nWorldY << 12;
	*(int*) ((char*) pObject + 0x48) = nWorldZ << 12;
	return pObject;
}

// MACINTOSH: schedule_coll_runtime_timer_0xfa_and_request_level_state_0x1e()
// FUNCTION: LEMBALL 0x00422c80
int LEMBALL_FASTCALL ScheduleCollRuntimeTimer0xfa(void)
{
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0xfa);
	*(int*) ((char*) g_pActiveManagedEntityOwner + 0xe8) = *(int*) ((char*) g_pActiveManagedEntityOwner + 0xe8) + 0x1e;
	return 1;
}

// MACINTOSH: notify_coll_contact_target_and_schedule_timer_0x9c4()
// FUNCTION: LEMBALL 0x00422c00
int LEMBALL_FASTCALL NotifyCollContactTargetAndScheduleTimer0x9c4(int pObject)
{
	(*( void(__fastcall**)(int, int)) (**(char***) ((char*) pObject + 0x5c) + 0xb8)) (*(int*) ((char*) pObject + 0x64), pObject);
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0x9c4);
	*(int*) ((char*) g_pActiveManagedEntityOwner + 0xfc) = *(int*) ((char*) g_pActiveManagedEntityOwner + 0xfc) - 1;
	return 1;
}

// MACINTOSH: complete_gmob_action_29()
// FUNCTION: LEMBALL 0x0041d5a0
void LEMBALL_FASTCALL CompleteGmobAction29(int* pObject)
{
	(*( void(__fastcall**)(int, int*)) (**(char***) ((char*) pObject[0x17]) + 0xb8)) (pObject[0x19], pObject);
	(*( void(__fastcall**)(int)) (*(char***) pObject + 0x34)) (0x29);
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(10);
}

// MACINTOSH: try_attach_gmob_target()
// FUNCTION: LEMBALL 0x0041d560
int LEMBALL_FASTCALL TryAttachGmobTarget(void* pObject, int* pTarget)
{
	*(int**) ((char*) pObject + 0x5c) = pTarget;
	if ((*( int(__fastcall**)(int)) (*(char***) pTarget + 0xb4)) (*(int*) ((char*) pObject + 0x64)) == 0) {
		((ManagedEntityStateView*) pObject)->RequestManagedEntityStateId(0x1a);
		return 1;
	}
	return 0;
}

// MACINTOSH: CEnemy::EnemyRule_RADIUS50()
// FUNCTION: LEMBALL 0x00420070
int LEMBALL_FASTCALL IsSavedTargetWithinProbeBoxes(void)
{
	if (*(int*) ((char*) g_pActiveManagedEntityOwner + 0x58) == 0) {
		return 0;
	}
	return ((int(__fastcall*)(int)) 0x40358f)(0x32);
}

// MACINTOSH: CSlinky::CSlinky() [type-0x35 chunk]
// FUNCTION: LEMBALL 0x0040b480
void* LEMBALL_FASTCALL ConstructType35ChunkObject(void* pObject)
{
	((CGameObject*) pObject)->ConstructCGameObject(0x35, 0, 0);
	*(int*) ((char*) pObject + 0x138) = 0;
	*(int*) ((char*) pObject + 0x13c) = 0;
	*(int*) ((char*) pObject + 0x140) = 0;
	*(int*) ((char*) pObject + 0x144) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x148) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x14c) = 0xaa55aa55;
	(* (void***) pObject) = (void**) 0x4932f8;
	return pObject;
}

// MACINTOSH: construct_positioned_level_chunk_object(int)
// FUNCTION: LEMBALL 0x0041c380
void* LEMBALL_FASTCALL ConstructPositionedLevelChunkObject(void* pObject, int nUnused, void* pPoint, int nType)
{
	((CGameObject*) pObject)->InitializeLevelChunkObjectBase(nType, 0, 0);
	(* (void***) pObject) = (void**) 0x493580;
	*(int*) ((char*) pObject + 0x138) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x13c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x140) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x138) = *(int*) pPoint;
	*(int*) ((char*) pObject + 0x13c) = *(int*) ((char*) pPoint + 4);
	*(int*) ((char*) pObject + 0x140) = *(int*) ((char*) pPoint + 8);
	return pObject;
}

// MACINTOSH: construct_linked_type_0x11_chunk_object(int, ushort) [type-0x11 linked chunk]
// FUNCTION: LEMBALL 0x0041c470
void* LEMBALL_FASTCALL ConstructLinkedType11ChunkObject(void* pObject, int nUnused, void* pPoint, int pLinked, unsigned short pId)
{
	((CGameObject*) pObject)->InitializeLevelChunkObjectBase(0x11, 0, 0);
	(* (void***) pObject) = (void**) 0x493580;
	*(int*) ((char*) pObject + 0x138) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x13c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x140) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x138) = *(int*) pPoint;
	*(int*) ((char*) pObject + 0x13c) = *(int*) ((char*) pPoint + 4);
	*(unsigned short*) ((char*) pObject + 0x144) = pId;
	*(int*) ((char*) pObject + 0x140) = *(int*) ((char*) pPoint + 8);
	(* (void***) pObject) = (void**) 0x4948d0;
	*(int*) ((char*) pObject + 0x148) = pLinked;
	if (pLinked == 0) {
		*(int*) ((char*) pObject + 0x14c) = 0xffff;
		return pObject;
	}
	*(int*) ((char*) pObject + 0x14c) = *(int*) ((char*) pLinked + 0x64);
	return pObject;
}

// MACINTOSH: CTrapDoor::CTrapDoor() [type-0x18 chunk]
// FUNCTION: LEMBALL 0x0040c2d0
void* LEMBALL_FASTCALL ConstructType18ChunkObject(void* pObject, int nUnused, void* pPoint, int pContext)
{
	((CGameObject*) pObject)->InitializeLevelChunkObjectBase(0x18, 0, 0);
	(* (void***) pObject) = (void**) 0x493580;
	*(int*) ((char*) pObject + 0x138) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x13c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x140) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x138) = *(int*) pPoint;
	*(int*) ((char*) pObject + 0x13c) = *(int*) ((char*) pPoint + 4);
	*(int*) ((char*) pObject + 0x140) = *(int*) ((char*) pPoint + 8);
	(* (void***) pObject) = (void**) 0x493438;
	*(int*) ((char*) pObject + 0x40) = *(int*) pPoint;
	*(int*) ((char*) pObject + 0x44) = *(int*) ((char*) pPoint + 4);
	*(int*) ((char*) pObject + 0x48) = *(int*) ((char*) pPoint + 8);
	*(int*) ((char*) pObject + 0x144) = pContext;
	return pObject;
}

// MACINTOSH: CBullet::CBullet() [type-3 projectile chunk]
// FUNCTION: LEMBALL 0x0041a510
void* LEMBALL_FASTCALL ConstructProjectileObject(void* pObject)
{
	((CGameObject*) pObject)->InitializeLevelChunkObjectBase(3, 0x100, 0);
	*(void**) ((char*) pObject + 0x138) = (void**) 0x4932c8;
	*(int*) ((char*) pObject + 0x13c) = 0;
	((void(__fastcall*)(void*)) 0x45ee80)((char*) pObject + 0x138);
	*(int*) ((char*) pObject + 0x188) = 0;
	*(int*) ((char*) pObject + 0x174) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x178) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x17c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x180) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x18c) = 0;
	*(int*) ((char*) pObject + 0x190) = 0;
	*(int*) ((char*) pObject + 0x194) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x198) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x19c) = 0xaa55aa55;
	(* (void***) pObject) = (void**) 0x4940f0;
	*(void**) ((char*) pObject + 0x138) = (void**) 0x4940d0;
	*(int*) ((char*) pObject + 0x150) = *(int*) ((char*) pObject + 0x150) + 0x28;
	return pObject;
}

// MACINTOSH: CPlayerLemmingGroupManager::DeleteGroup()
// FUNCTION: LEMBALL 0x004187f0
int LEMBALL_FASTCALL RemoveGroupChildOrSelectPrevious(void* pGroup, int nUnused, void* param_2, int* param_3)
{
	int iResult = ((int(__fastcall*)(void*, void*, int*)) 0x401cbc)(pGroup, param_2, param_3);
	if (iResult == 0) {
		((void(__fastcall*)(void*)) 0x402d47)(pGroup);
	}
	return iResult;
}
