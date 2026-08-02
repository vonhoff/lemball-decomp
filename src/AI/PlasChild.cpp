#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Mixed/Level/CHUNKOBJVT.H"

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct CPlayerLemmingGroup : LevelChunkObjectBaseView {
	CPlayerLemmingGroup(int nType, unsigned short nVariant, unsigned short nStateId);
};

extern void* g_LEVELVT_LevelChunkObjectChildrenVariantVtable[16];

// Split from LEVELVT.CPP to preserve compiler state in original translation unit.

// MACINTOSH: CPlayerLemmingGroup::CPlayerLemmingGroup(CAI*, CObjectManager*, CFormationManager*)
// FUNCTION: LEMBALL 0x00414010
CPlayerLemmingGroup::CPlayerLemmingGroup(int nType,
																			   unsigned short nVariant,
																			   unsigned short nStateId)
{
	InitializeLevelChunkObjectBase(nType, nVariant, nStateId);
	*(void**) this = g_LEVELVT_LevelChunkObjectChildrenVariantVtable;
}
