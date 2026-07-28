#include "ENGINE/CORE/VSINIT.H"

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
