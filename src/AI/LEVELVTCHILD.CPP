#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct ManagedEntityChildView {
	void SetManagedEntityOwnerGroup(void* pOwnerGroup);
	void SetManagedEntityPrimaryActiveFlag(int fActive);
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
	void AppendChild(void* pChild);
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
	int HasStreamDirtyChildren(void);
	int AttachManagedEntityChild(ManagedEntityChildView* pChild);
};

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation in LEVELVT.CPP.

// FUNCTION: LEMBALL 0x00414600
int LevelManagedEntityChildIterator::AttachManagedEntityChild(ManagedEntityChildView* pChild)
{
	typedef int(LEMBALL_FASTCALL * GetChildCountProc)(void*);
	AppendChild(pChild);
	m_fChildrenDirty164 = 1;
	pChild->SetManagedEntityOwnerGroup(this);
	if (((GetChildCountProc) (*(void***) this)[66])(this) == 0 && m_nActiveState168 == 1) {
		pChild->SetManagedEntityPrimaryActiveFlag(1);
		return 1;
	}
	pChild->SetManagedEntityPrimaryActiveFlag(0);
	return 1;
}
