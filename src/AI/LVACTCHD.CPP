#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelManagedEntityChildIterator;

struct LevelManagedEntityActiveGroupView {
	LevelManagedEntityChildIterator* GetFirstActiveEntityThunk(void);
};

typedef void(LEMBALL_FASTCALL* LevelManagedEntityNoArgProc)(void* pEntity);

// FUNCTION: LEMBALL 0x00418a90
void LEMBALL_FASTCALL ClearFirstActiveManagedEntityPendingState(LevelManagedEntityActiveGroupView* pGroup)
{
	void* pEntity = pGroup->GetFirstActiveEntityThunk();
	if (pEntity != 0) {
		void** pVtable = *(void***) pEntity;
		((LevelManagedEntityNoArgProc) pVtable[0x150 / sizeof(void*)])(pEntity);
	}
}
