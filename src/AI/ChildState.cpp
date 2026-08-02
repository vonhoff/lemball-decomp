#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

typedef void*(LEMBALL_FASTCALL* GetManagedEntityChildProc)(void* pObject);
typedef int(LEMBALL_FASTCALL* GetManagedEntityChildStatusProc)(void* pObject);

struct CachedGroupOwnerVtableView {
	void* m_pReserved00;
	GetManagedEntityChildProc m_pGetFirstCachedChild04;
	void* m_apReserved08[2];
	GetManagedEntityChildProc m_pGetFirstIteratedChild10;
};

struct CachedGroupOwnerView {
	CachedGroupOwnerVtableView* m_pVtable00;
};

struct ManagedEntityChildStatusVtableView {
	void* m_apReserved00[0x45];
	GetManagedEntityChildStatusProc m_pGetCachedChild114;
	void* m_pReserved118;
	GetManagedEntityChildStatusProc m_pGetIteratedChild11C;
};

struct ManagedEntityChildStatusView {
	ManagedEntityChildStatusVtableView* m_pVtable00;
};

int LEMBALL_FASTCALL GetFirstManagedEntityChildFromCachedGroup(void* pObject)
{
	ManagedEntityChildStatusView* pChild;

	pChild = (ManagedEntityChildStatusView*) ((CachedGroupOwnerView*) pObject)->m_pVtable00
		->m_pGetFirstCachedChild04(pObject);
	return pChild != 0 ? pChild->m_pVtable00->m_pGetCachedChild114(pChild) : 0;
}

int LEMBALL_FASTCALL GetFirstIteratedChildStatus11C(void* pObject)
{
	ManagedEntityChildStatusView* pChild;
	int nStatus;

	nStatus = 0;
	pChild = (ManagedEntityChildStatusView*) ((CachedGroupOwnerView*) pObject)->m_pVtable00
		->m_pGetFirstIteratedChild10(pObject);
	if (pChild != 0) {
		nStatus = pChild->m_pVtable00->m_pGetIteratedChild11C(pChild);
	}
	return nStatus;
}
