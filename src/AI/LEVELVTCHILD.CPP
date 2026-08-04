#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/CGameObject.h"

struct ManagedEntityChildView {
	void SetManagedEntityOwnerGroup(void* pOwnerGroup);
	void SetManagedEntityPrimaryActiveFlag(int fActive);
};

struct CGenericGroup {
	void AddElementToGroup(CGameObject* pChild);
};


struct LevelManagedEntityChildIterator {
	char m_abReserved00[0x124];
	int m_cChildren124;
	void* m_apChildren128[10];
	int m_iCurrentChild150;
	char m_abReserved154[0x10];
	int m_fChildrenDirty164;
	int m_nActiveState168;

	int GetChildCount(void);
	void* GetFirstChild(void);
	void* GetNextChild(void);
	void* GetCurrentChild(void);
	void* GetChildAt(int iChild);
	int ContainsChild(void* pChild);
	void* GetFirstChildThunk(void);
	void* GetNextChildThunk(void);
	void* GetCurrentChildThunk(void);
	int GetActiveState(void);
	int GetActiveStateThunk(void);
	void* GetChildWithState8(void);
	void* GetChildWithState8Thunk(void);
	void* GetCurrentChildWithState8(void);
	int HasDirtyChildren(void);
	int HasDirtyChildrenThunk(void);
	int HasSFXChanged(void);
	int AddLemmingToGroup(ManagedEntityChildView* pChild);
};

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation in LEVELVT.CPP.

// MACINTOSH: CPlayerLemmingGroup::AddLemmingToGroup(CPlayerLemming*)
// FUNCTION: LEMBALL 0x00414600
int LevelManagedEntityChildIterator::AddLemmingToGroup(ManagedEntityChildView* pChild)
{
	typedef int(LEMBALL_FASTCALL * GetChildCountProc)(void*);
	((CGenericGroup*) this)->AddElementToGroup((CGameObject*) pChild);
	m_fChildrenDirty164 = 1;
	pChild->SetManagedEntityOwnerGroup(this);
	if (((GetChildCountProc) (*(void***) this)[66])(this) == 0 && m_nActiveState168 == 1) {
		pChild->SetManagedEntityPrimaryActiveFlag(1);
		return 1;
	}
	pChild->SetManagedEntityPrimaryActiveFlag(0);
	return 1;
}
