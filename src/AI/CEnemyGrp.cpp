#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "AI/CGenericGroup.h"

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct EnmyManagedEntityGroupView : LevelChunkObjectBaseView {
	void* m_pVtable00;
	char m_abReserved04[0x120];

	EnmyManagedEntityGroupView* ConstructEnmyManagedEntityGroup(int nType,
																unsigned short nVariant,
																unsigned short nStateId);
};

extern CGenericGroupChunkVtable g_LEVELVT_EnmyManagedEntityGroupVtable;

struct CEnemyGroup : EnmyManagedEntityGroupView {
	CEnemyGroup(int nAI, unsigned short nObjectManager, unsigned short nFormationManager);
};

// FUNCTION: LEMBALL 0x00420ae0
CEnemyGroup::CEnemyGroup(int nAI, unsigned short nObjectManager, unsigned short nFormationManager)
{
	InitializeLevelChunkObjectBase(nAI, nObjectManager, nFormationManager);
	*(void**) this = &g_LEVELVT_EnmyManagedEntityGroupVtable;
}
