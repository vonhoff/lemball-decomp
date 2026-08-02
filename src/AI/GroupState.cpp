#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

void LEMBALL_FASTCALL ActivateIteratedLevelChunkChildren(void* pIterator);

struct ManagedEntityGroupDetachCacheView {
	char m_abReserved00[0x70];
	int m_fCached70;
};

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation in LINKSCF.CPP.

// FUNCTION: LEMBALL 0x00418520
void LEMBALL_FASTCALL ResetManagedEntityGroupDetachCache(void* pCache)
{
	ManagedEntityGroupDetachCacheView* pDetachCache;

	pDetachCache = (ManagedEntityGroupDetachCacheView*) pCache;
	ActivateIteratedLevelChunkChildren((char*) pDetachCache - 0xb0);
	pDetachCache->m_fCached70 = 0;
}

struct ManagedEntityGroupChildRuntimeView {
	char m_abReserved00[0x38];
	int m_fActive38;
};

struct ManagedEntityGroupChildrenView {
	char m_abReserved00[0x3c];
	ManagedEntityGroupChildRuntimeView** m_ppChildren3C;

	void ClearChildActiveFlag(int iChild);
};

// FUNCTION: LEMBALL 0x0041b990
void ManagedEntityGroupChildrenView::ClearChildActiveFlag(int iChild)
{
	m_ppChildren3C[iChild]->m_fActive38 = 0;
}
