#include "AI/CPlayerLemmingGroupManager.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelManagedEntityActiveGroupView;

typedef void(LEMBALL_FASTCALL* LevelManagedEntityNoArgProc)(void* pEntity);

// MACINTOSH: CPlayerLemmingGroupManager::RemoveWaypointsFromCurrentGroup()
// FUNCTION: LEMBALL 0x00418a90
void LEMBALL_FASTCALL ClearFirstActiveManagedEntityPendingState(LevelManagedEntityActiveGroupView* pGroup)
{
	void* pEntity = ((CPlayerLemmingGroupManager*) pGroup)->GetPlayerControlledGroup();
	if (pEntity != 0) {
		void** pVtable = *(void***) pEntity;
		((LevelManagedEntityNoArgProc) pVtable[0x150 / sizeof(void*)])(pEntity);
	}
}
